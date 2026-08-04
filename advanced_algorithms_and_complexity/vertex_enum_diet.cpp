#include<iostream>
#include<vector>
#include<utility>
#include<iomanip>
#include<algorithm>

using namespace std;

class row{
    public:
        row(vector<double> a,int i = -1):arr(a),pivot_index(i){}
        row(const row& other){
            arr = other.arr;
            pivot_index = other.pivot_index;
        }
        row() = default;
        double& operator[](int i){
            return arr[i];
        }
        const double& operator[](int i)const {
            return arr[i];
        }
        int size()const{
            return arr.size();
        }
        vector<double>arr;
        int pivot_index;
};

class graph{
    public:
        graph(vector<row>m):matrix(m),first_non_pivot_row(0){}

        row& operator[](int i){
            return matrix[i];
        }
        const row& operator[](int i)const{
            return matrix[i];
        }

        int size()const{
            return matrix.size();
        }

        vector<row>matrix;
        int first_non_pivot_row;

};


pair<int,int> find_left_most_non_zero_in_non_pivot(const graph& g){
    int top = g.first_non_pivot_row;
    int size = g.size();
    int row_size = size + 1;
    int best_row = -1;
    int first_non_zero = -1;
    for(int i = top; i<size; i++){

        for(int j = 0; j<row_size; j++){
            if(abs(g[i][j]) > 1e-7){
                if(j<first_non_zero || first_non_zero == -1){
                    first_non_zero = j;
                    best_row = i;
                }
                break;
            }
            if(j>=first_non_zero && first_non_zero != -1){break;}
        }
    }
    return {best_row,first_non_zero};
}


void swap_to_top(int i,graph& g){
    int top = g.first_non_pivot_row;
    row temp = g[i];
    g[i] = g[top];
    g[top] = temp;
}

void resize_row(int best_row,int best_index,graph& g){
    double pivot_size = g[best_row][best_index];
    int row_size = g.size() + 1;
    for(int i = 0; i<row_size; i++){
        g[best_row][i] /= pivot_size;
    }
}

void subtruct_row_from_others(int best_row,int pivot_index,graph& g){
    int size = g.size();
    int row_size = size +1;
    for(int i = 0; i<size; i++){
        double subtruct_count = g[i][pivot_index];
        if(i == best_row){continue;}
        for(int j = 0; j<row_size; j++){
            g[i][j] -= g[best_row][j] * subtruct_count;
        }
    }
}

vector<double> read_answers(const graph& g){
    int size = g.size();
    int row_size = size +1;
    vector<double>results;
    for(int i = 0; i<size; i++){
        int pivot_index = g[i].pivot_index;
        if(pivot_index == row_size -1){continue;}
        double result = g[i][size];
        for(int j = 0; j<size; j++){
            if(j == pivot_index){continue;}
            //acting as other variables if they exist are equal 1
            result -= g[i][j];
        }
        results.push_back(result); 
    }
    return results;
}

vector<double> guassian_elimination(graph& g){
    int graph_size = g.size();
    int pivot_index = 0;;
    int best_row = 0;
    pair<int,int> best_row_and_index;

    while(g.first_non_pivot_row != graph_size && pivot_index != -1){

        best_row_and_index = find_left_most_non_zero_in_non_pivot(g);
        best_row = best_row_and_index.first;
        pivot_index = best_row_and_index.second;

        if(pivot_index == -1){break;}

        g[best_row].pivot_index = pivot_index;

        resize_row(best_row,pivot_index,g);

        subtruct_row_from_others(best_row,pivot_index,g);

        swap_to_top(best_row,g);

        g.first_non_pivot_row += 1;
    }

    return read_answers(g);
}



vector<vector<int>> compute_combinations(int inequal_count, int var_count){
    vector<vector<int>>combinations;

    vector<int>bitmask(inequal_count,0);
    for(int i = 0; i<var_count; i++){
        bitmask[i] = 1;
    }
    bool has_comb_left = true;
    while(has_comb_left){
        vector<int> curr_subset;
        //if the inf bound (the last inequality) is in the combination it will be last
        // becuase it places them in sorted order
        for(int i = 0; i<inequal_count; i++){
            if(bitmask[i] == 1){
                curr_subset.push_back(i);
            }
        }
        combinations.push_back(curr_subset);
        //moves the 1's around to generate all combinations
        //and returns false if cant create another subset
        has_comb_left = prev_permutation(bitmask.begin(),bitmask.end());
    }
    return combinations;
}

bool satisfies_all_inequal(const vector<double>&x,const vector<vector<double>>&inequalities){
    int inequal_count = inequalities.size();
    int var_count = x.size();
    for(int i = 0; i<inequal_count; i++){
        double left_side = 0.0;
        double right_side = inequalities[i].back();
        for(int j = 0; j<var_count; j++){
            left_side += inequalities[i][j] * x[j];
        }
        if(left_side > right_side + 1e-3){
            return false;
        }
    }
    return true;
}


double compute_happiness(const vector<int>& happy_values,vector<double>results){
    double happiness = 0.0;
    int var_count = happy_values.size();
    for(int i = 0; i<var_count; i++){
        happiness += happy_values[i]*results[i];
    }
    return happiness;
}


vector<row> create_graph_from_inequalities(
    const vector<int>& curr_combination,
    const vector<vector<double>>&inequalities
){
    vector<row>matrix;
    for(const int& i:curr_combination){
        row r(inequalities[i]);
        matrix.push_back(r);
    }
    return matrix;
}

vector<double> best_diet(
    const vector<vector<double>>&inequalities,
    const vector<int>& happy_values,bool& in_bound
){
    int inequal_count = inequalities.size();
    int var_count = happy_values.size();
    int inf_bound_indx = inequal_count - 1;
    vector<double>best_vars;
    double max_happiness = -1e10;
    bool best_in_bound = true;
    vector<vector<int>>inequal_combinations = compute_combinations(inequal_count,var_count);
    int combinations_count = inequal_combinations.size();
    for(int i = 0; i<combinations_count; i++){
        vector<int>curr_combination = inequal_combinations[i];
        //if the inf bound (the last inequality) is in the combination it will be  the last one
        bool uses_inf_bound = (!curr_combination.empty() && curr_combination.back() == inf_bound_indx);
        vector<row> matrix = create_graph_from_inequalities(curr_combination,inequalities);
        graph g(matrix);
        vector<double>results = guassian_elimination(g);
        if(results.size() == var_count && satisfies_all_inequal(results,inequalities)){
            double curr_happiness = compute_happiness(happy_values,results);
            if(curr_happiness > max_happiness){
                max_happiness = curr_happiness;
                best_vars = results;
                best_in_bound = !uses_inf_bound;
            }
        }
    }
    in_bound = best_in_bound;
    return best_vars;
}


void print_solution(const vector<double>&solution){
    if(solution.empty()){
        cout<<"No solution";
        return;
    }
    cout<<"Bounded solution\n";
    for(int i = 0; i<solution.size(); i++){
        cout<<solution[i]<<" ";
    }
}


int main(){
    cout << fixed << setprecision(18);
    int inequal_count,var_count;
    cin>>inequal_count>>var_count;
    vector<vector<double>>inequalities(inequal_count,vector<double>(var_count+1));
    for(int i = 0; i<inequal_count; i++){
        for(int j = 0; j<var_count; j++){
            cin>>inequalities[i][j];
        }
    }
    for(int i = 0; i<inequal_count; i++){
        cin>>inequalities[i][var_count];
    }
    vector<int>happy_values(var_count);
    for(int i = 0; i<var_count; i++){
        cin>>happy_values[i];
    }

    //bound for non negetive solutions
    for (int i = 0; i < var_count; i++) {
        vector<double> inequal(var_count + 1, 0.0);
        inequal[i] = -1.0;
        inequal[var_count] = 0.0;
        inequalities.push_back(inequal);
    }

    vector<double> inf_bound(var_count + 1, 1);
    inf_bound[var_count] = 1e9;
    inequalities.push_back(inf_bound);
    bool in_bound = true;
    vector<double>solution = best_diet(inequalities,happy_values,in_bound);
    if(in_bound){
        print_solution(solution);
    }
    else{
        cout<<"Infinity";
    }
    
}


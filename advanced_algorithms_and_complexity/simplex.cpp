#include<iostream>
#include<vector>
#include<utility>
#include<iomanip>
#include<algorithm>

using namespace std;


//I have ZERO ide whats happening here. Simplex is very relient on Linear Algebra which I never learned
//So I relied on Gemini, not copying but trying my best to understand. 
// Still this is way out of bounds for me to truely understand


struct inequality{
    inequality(vector<double>a,double b):arr(a),bound(b){}
    inequality(const inequality&other){
        arr = other.arr;
        bound = other.bound;
    }
    inequality(int size){
        arr.resize(size);
    }
    vector<double>arr;
    double bound;
};

class row{
    public:
        row(vector<double> a,int i = -1):arr(a),pivot_index(i){}

        row(inequality inequal):pivot_index(-1){
            arr = inequal.arr;
            arr.push_back(inequal.bound);
        }
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
    int row_size = g[0].size();
    int best_row = -1;
    int first_non_zero = -1;
    for(int i = top; i<size; i++){

        for(int j = 0; j<row_size; j++){
            if(g[i][j] != 0){
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
    int row_size = g[0].size();
    for(int i = 0; i<row_size; i++){
        g[best_row][i] /= pivot_size;
    }
}

void subtruct_row_from_others(int best_row,int pivot_index,graph& g){
    int size = g.size();
    int row_size = g[0].size();
    for(int i = 0; i<size; i++){
        double subtruct_count = g[i][pivot_index];
        if(i == best_row || subtruct_count == 0.0){continue;}
        for(int j = 0; j<row_size; j++){
            g[i][j] -= g[best_row][j] * subtruct_count;
        }
    }
}


vector<double> read_answers(const graph& g){
    int size = g.size();
    int row_size = g[0].size();
    vector<double>results;
    for(int i = 0; i<size; i++){
        int pivot_index = g[i].pivot_index;
        if(pivot_index == size){continue;}
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

//**************************************************************************************************************
//**************************************************************************************************************
//this video helped me understand it better https://www.youtube.com/watch?v=dO1pV9qaHs8
vector<vector<double>> init_inv_a(
    const vector<inequality>&inequalities,
    const vector<int>&touching_inequalities
){
    int size = touching_inequalities.size();
    vector<vector<double>> inv_a(size,vector<double>(size,0.0));
    vector<row>matrix;
    for(int i = 0; i<size; i++){
        //because inv_a * a = I(the matrix with just 1 at pos i) I set for the guassian elimination
        //the rows to be 0 other then at size_i which is 1
        //this will resualt in the inv_a or the inverse of the inequalities
        //because it gets the vector_i in which it pushes off wall i by 1 units
        int wall_indx = touching_inequalities[i];
        inequality curr_row = inequalities[wall_indx];
        //keeps original values and adds |size| 0's after
        curr_row.arr.resize(2*size,0.0);
        curr_row.arr[size+i] = 1;
        matrix.push_back(row(curr_row));
    }
    graph g(matrix);
    vector<double> col_i = guassian_elimination(g);
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++){
            inv_a[i][j] = g[i][size+j];
        }
    }
    return inv_a;
}
//**************************************************************************************************************
//**************************************************************************************************************





double dot_product(const vector<double>& a,const vector<double>&b){
    double val = 0.0;
    int var_count = a.size();
    for(int i = 0; i<var_count; i++){
        val += a[i]*b[i];
    }
    return val;
}

vector<double> get_edge_dir(
    const int& relaxed_wall, 
    const vector<vector<double>>&inv_a
){
     //makes the elimination look deeper into this direction
    //original in equality => A * p <= b
    // A * p = b because im on one of the vertices
    // so if I go further in the direction it has to be lower than b
    // A * w(dir) + A * p < b 
    // A * w < 0 => so i set it to -1
//**************************************************************************************************************
//**************************************************************************************************************
    int var_count = inv_a.size();
    vector<double> edge_dir(var_count);

    for(int i = 0; i<var_count; i++){
        //
        edge_dir[i] = -inv_a[i][relaxed_wall];
    }
//**************************************************************************************************************
//**************************************************************************************************************
    return edge_dir;
}

//finds the next vertex by walking along an edge direction until hitting the nearest new wall
vector<double> other_edge_end(
    const vector<double>&point,
    const vector<inequality>&inequalities,
    const vector<int>&touching_inequalities,
    int relaxed_wall,
    double& step_dist,
    int& new_inequal_indx,
    vector<vector<double>>&inv_a
){
    vector<double> edge_dir = get_edge_dir(relaxed_wall,inv_a);

    step_dist = 1e15;
    new_inequal_indx = -1;

    int inequal_count = inequalities.size();
    vector<bool>is_touching(inequal_count,false);
    for(const int& i:touching_inequalities){
        is_touching[i] = true;
    }
    
    for(int i = 0; i<inequal_count; i++){
        //no need to look at walls im already touching 
        // because I need to find the next closest wall 
        if(is_touching[i]){continue;}
        //gets the direction similarity of the direction vector and the cannidate wall
        //if its negetive then the wall is in the other direction so I can skip it
        //if its 0 then its perpendicular to the current wall so I could never reach it
        //only if its positive then I could reach it by walking along the wall
        //this calculates the rate of approch or the speed traveling to that wall
        double dir_to_wall_dot_product = dot_product(inequalities[i].arr,edge_dir);
        if(dir_to_wall_dot_product>1e-7){
            double dist_from_wall = dot_product(inequalities[i].arr,point);
            double bound = inequalities[i].bound;
            double gap_left = bound - dist_from_wall;
            if(gap_left<0){gap_left = 0.0;}
            double step_count = gap_left/dir_to_wall_dot_product;
            //we want to find the closest wall so we find the lowest step_dist
            if(step_count<step_dist){
                step_dist = step_count;
                new_inequal_indx = i;
            }
            //optimization prefering earlier of the same step results
            else if(abs(step_count - step_dist) <= 1e-11){
                if(new_inequal_indx == -1 || i < new_inequal_indx){
                    new_inequal_indx = i;
                }
            }
        }

    }
    if(new_inequal_indx == -1){return{};}
    int point_size = point.size();
    vector<double>new_point(point_size);
    for(int i = 0; i<point_size; i++){
        new_point[i] = point[i] + step_dist * edge_dir[i];
    }
    return (new_point);
}



//Returns the index of the best active wall to drop to increase objective score.
//Returns -1 if all multipliers are positive (meaning we are already at the optimal
int get_best_wall(
    const vector<int>&touching_inequalities,
    const vector<inequality>&inequalities,
    const vector<double>happy_values,
    const vector<vector<double>>& inv_a
){
    int best_inequal = -1;
    int var_count = happy_values.size();


//Solves A^T * lambda = happy_values  -->  lambda = (A^-1)^T * happy_values.
//
//var_results[i] represents lambda_i for touching_inequalities[i]:
//lambda_i = dot_product( Column i of inv_a, happy_values )
// 
//If lambda_i < 0: Leaving wall i and walking along -inv_a[:][i] INCREASES objective value.
//If all lambda_i >= 0: Every direction off the current vertex decreases happiness.
//We are at the OPTIMAL VERTEX!
//var_results[i] is the multiplier (lambda_i) for wall touching_inequalities[i]

    vector<double> var_results(var_count,0);
    for(int i = 0; i<var_count; i++){
        for(int j = 0; j<var_count; j++){
            var_results[i] += inv_a[j][i]*happy_values[j];
        }
    }


    double min_var_result = -1e-7;
    for(int i = 0; i<var_count; i++){
        //if all are positive I am at the best vertex already
        //because it means that stepping away from any of the walls would lower my happiness
        // otherwise choosing the min would mean that when I leave that wall behind
        // I get the most happiness
        if(min_var_result > var_results[i]){
            min_var_result = var_results[i];
            best_inequal = touching_inequalities[i];
        }
        //optimization prefering earlier of the same weight results
        else if(abs(var_results[i] - min_var_result)<=1e-11){
            if(best_inequal == -1 || touching_inequalities[i] < best_inequal){
                best_inequal = touching_inequalities[i];
            }
        }
    }
    //If no multiplier was negative (best_inequal remains -1), all current walls 
    //are supporting our position. Dropping any wall would decrease our objective score,
    //so we are at the optimal vertex!
    return best_inequal;
}

struct simplex_solution{
    simplex_solution(vector<double>a,bool b):arr(a),is_bounded(b){}
    vector<double>arr;
    bool is_bounded;
};

simplex_solution simplex(
    const vector<inequality>&inequalities,
    const vector<double>&happy_values,
    vector<double>&point,
    vector<int>&touching_inequalities

){
    int inequal_count = inequalities.size();
    int var_count = happy_values.size();
    vector<vector<double>> inv_a = init_inv_a(inequalities,touching_inequalities);

    if(point.empty()){
        return {{},true};
    }
    int iterations = 0;
    while(true){
        iterations++;
        if(iterations>50000){
            return {{},false};
        }
        int relaxed_r = get_best_wall(touching_inequalities,inequalities,happy_values,inv_a);
        if(relaxed_r == -1){
            //at optimal vertex
            return {point,true};
        }

        int relaxed_wall = -1;
        for(int i = 0; i < var_count; i++){
            if(touching_inequalities[i] == relaxed_r){
                relaxed_wall = i;
                break;
            }
        }

        double min_steps = 0;
        int new_wall = -1;
        vector<double> new_point = other_edge_end(
            point,
            inequalities,
            touching_inequalities,
            relaxed_wall,
            min_steps,
            new_wall,
            inv_a
        );
        
        if(new_point.empty() || new_wall == -1){
            return {{},false};
        }

        point = new_point;

        //update the touching array to replace the old wall with the new one
        for(int i = 0; i<var_count; i++){
            if(touching_inequalities[i] == relaxed_r){
                touching_inequalities[i] = new_wall;
                break;
            }
        }
        inv_a = init_inv_a(inequalities,touching_inequalities);

    }
}

void print_solution(const simplex_solution&solution){
    if(!solution.is_bounded){
        cout<<"Infinity";
        return;
    }
    else if(solution.arr.empty()){
        cout<<"No solution";
        return;
    }
    cout<<"Bounded solution\n";
    for(int i = 0; i<solution.arr.size(); i++){
        cout<<solution.arr[i]<<" ";
    }
}

void add_non_neg_cons(vector<inequality>&inequalities,const int& var_count){
    for (int i = 0; i<var_count; i++) {
        inequality inequal(vector<double>(var_count,0), 0.0);
        inequal.arr[i] = -1.0;
        inequalities.push_back(inequal);
    }
}

void start_simplex(
    vector<inequality>&inequalities,
    const vector<double>&happy_values,
    vector<double>&point,
    vector<int>&touching_inequalities
){
    int var_count = happy_values.size();
    simplex_solution solution = simplex(
        inequalities,
        happy_values,
        point,
        touching_inequalities
    );
    print_solution(solution);
}


vector<inequality> create_augmented_matrix(
    const vector<inequality>&inequalities,
    const vector<double>&happy_values
){
    vector<inequality>augmented_inequalities = inequalities;
    int var_count = happy_values.size();
    int inequal_count = inequalities.size();
    for(int i = 0; i<inequal_count; i++){
        //augmenting the graph to find a common point
        augmented_inequalities[i].arr.push_back(-1.0);
    }
    //add non negetive bound for augmented matrix
    add_non_neg_cons(augmented_inequalities,var_count+1);
    return augmented_inequalities;
}


bool init_fake_start_vertex(
    vector<inequality>&inequalities,
    const vector<double>&happy_values,
    vector<double>& fake_point,
    vector<int>&fake_p_touching
){
    fake_p_touching.clear();
    int var_count = happy_values.size();
    int inequal_count = inequalities.size();
    //find inequality with the most negative bound
    int min_bound_index = -1;
    double min_bound = 0;
    for(int i = 0; i<inequal_count; i++){
        if(inequalities[i].bound<min_bound){
            min_bound = inequalities[i].bound;
            min_bound_index = i;
        }
    }
    if(min_bound_index == -1){return false;}
    fake_point.resize(var_count+1,0);
    fake_point[var_count] = -min_bound; //setting the fake var I added

    /************************************************************************************************
    cout<<"\nz is equal: "<<-min_bound<<"\n";
    *************************************************************************************************/

    //definetly a wall fake_point is touching because it came from there
    fake_p_touching.push_back(min_bound_index);
    for(int i = 0; i<var_count; i++){
        //all of the non negetive constraints I added (-x_i <=0)
        fake_p_touching.push_back(inequal_count+i);
    }
    return true;
}


void init_simplex(
    vector<inequality>&inequalities,
    const vector<double>&happy_values
){
    int var_count = happy_values.size();
    int inequal_count = inequalities.size();

    vector<double> fake_point;
    vector<int>fake_p_touching;

    bool has_neg_b = init_fake_start_vertex(inequalities,happy_values,fake_point,fake_p_touching);

    if(!has_neg_b){
        vector<double> default_point(var_count,0);
        vector<int>start_touching;
        add_non_neg_cons(inequalities, var_count);
        for(int i = 0; i<var_count; i++){
            start_touching.push_back(inequal_count+i);
        }
        start_simplex(inequalities,happy_values,default_point,start_touching);
        return;
    }

    vector<inequality>augmented_inequalities = create_augmented_matrix(inequalities,happy_values);
    vector<double>fake_happy_values(var_count+1,0);
    fake_happy_values[var_count] = -1; //to maximize z to 0

    simplex_solution fake_sol = simplex(augmented_inequalities,fake_happy_values,fake_point,fake_p_touching);

    if(!fake_sol.is_bounded || fake_sol.arr.empty() || fake_point[var_count] > 1e-6){
        cout<<"No solution";
        return;
    }

    vector<double> real_point(fake_point.begin(), fake_point.begin() + var_count);

    add_non_neg_cons(inequalities, var_count);

    vector<int> real_touching;
    int z_wall = inequal_count + var_count;
    for(const int& wall:fake_p_touching){
        if(wall != z_wall){
            real_touching.push_back(wall);
        }
    }

    if(real_touching.size()>var_count){
        vector<int>pruned_touching;
        for(const int& wall:real_touching){
            bool is_opposite = false;
            for(int& added_wall:pruned_touching){
                bool match = true;
                for(int i = 0; i<var_count; i++){
                    if( abs(inequalities[wall].arr[i] + (inequalities[added_wall].arr[i])) > 1e-6 ){
                        match = false;
                        break;
                    }
                }
                if(match){
                    is_opposite = true;
                    break;
                }
            }
            if(!is_opposite){
                pruned_touching.push_back(wall);
                if(pruned_touching.size() == var_count){break;}
            }
        }
        real_touching = pruned_touching;
    }
     

    /************************************************************************************************
    if(real_touching.size()>var_count){
        cout<<"\n*********error********\n";
        for(int i:real_touching){cout<<i<<" ";}
    }
    **************************************************************************************************/

    start_simplex(inequalities,happy_values,real_point,real_touching);

}


int main(){
    cout << fixed << setprecision(18);
    int inequal_count,var_count;
    cin>>inequal_count>>var_count;
    vector<inequality>inequalities(inequal_count,inequality(var_count));
    for(int i = 0; i<inequal_count; i++){
        for(int j = 0; j<var_count; j++){
            cin>>inequalities[i].arr[j];
        }
    }
    for(int i = 0; i<inequal_count; i++){
        cin>>inequalities[i].bound;
    }
    vector<double>happy_values(var_count);
    for(int i = 0; i<var_count; i++){
        cin>>happy_values[i];
    }

    init_simplex(inequalities,happy_values);
    
}

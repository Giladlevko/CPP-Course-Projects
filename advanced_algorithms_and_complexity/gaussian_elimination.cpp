#include<iostream>
#include<vector>
#include<utility>
#include <iomanip>

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

void read_answers(const graph& g){
    int size = g.size();
    int row_size = size +1;
    for(int i = 0; i<size; i++){
        int pivot_index = g[i].pivot_index;
        if(pivot_index == row_size -1){continue;}
        double result = g[i][size];
        for(int j = 0; j<size; j++){
            if(j == pivot_index){continue;}
            //acting as other variables if they exist are equal 1
            result -= g[i][j];
        }
        cout<<result<<" "; 
    }
}

void guassian_elimination(graph& g){
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

    read_answers(g);
}


int main(){
    cout << fixed << setprecision(6);
    int col_count;
    cin>>col_count;
    //it is given that the row_count == col_count +1
    int row_count = col_count+1;
    vector<row> g_matrix(col_count);
    for(int i = 0; i<col_count; i++){
        vector<double> r(row_count);
        for(int j = 0; j<row_count; j++){
            cin>>r[j];
        }
        g_matrix[i] = row(r);
    }
    graph g(g_matrix);
    guassian_elimination(g);
}
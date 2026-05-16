#include<iostream>
#include<vector>
#include<iomanip>
using namespace std;

enum class hex_color{NONE,RED,BLUE};

ostream& operator<<(ostream& out,const hex_color& c){
    switch(c){
        case hex_color:: RED: out<<"RED";break;
        case hex_color:: BLUE: out<<"RED";break;
        case hex_color:: NONE: out<<"RED";break;
        default:out<<"UNKNOWN";break;
    }
    return out;
}

class hex_cell{
    public:
        hex_cell( int r = 0,int colunm = 0, hex_color c = hex_color::NONE):
        row(r),col(colunm),color(c){}
        void set_color(hex_color c){color = c;}
        hex_color get_color()const{return color;}
        void set_row(int r){row = r;}
        void set_col(int c){col = c;}
        int get_row()const{return row;}
        int get_col()const{return col;}
    private:
        hex_color color;
        int col;
        int row;
};

class hex_graph{
    public:
        hex_graph(){
            int count = 0;
            edge_list.resize(121);
            board.resize(11, vector<hex_cell>(11));
            for (int i = 0;i<11;i++){
                for(int j = 0; j<11;j++){
                    make_cell(i,j,edge_list[count++]);
                }
            }
            for (int i = 0;i<11;i++){
                for (int j = 0; j<11;j++){
                    board[i][j] = hex_cell(i, j);
                }
            }

        }
        void print_graph(){
            const int TOTAL_NODES = 121;
            const int N = 11;
            for(int k = 0;k<TOTAL_NODES;k++){
                int i = k/N;
                int j = k%N;
                cout<<"( "<<i<<", "<<j<<" ): ";
                for(int cell_num:edge_list[k]){
                    int cell_i = cell_num/N;
                    int cell_j = cell_num%N;
                    cout<<"( "<<cell_i<<", "<<cell_j<<" ),";
                }
                cout<<endl;
            }
        }

        void print_board(){
            cout<<endl<<endl;
            for (int i = 0;i<11;i++){
                for (hex_cell cell:board[i]){
                    if (cell.get_color() == hex_color::NONE){
                        cout<<setw(8)<<"("+to_string(cell.get_row())+","+to_string(cell.get_col())+")";
                    }
                    else{
                        cout<<"("<<cell.get_color()<<"), ";
                    }
                }
                cout<<endl<<endl;
                for (int j = 0; j<=i;j++){
                    cout<<" ";
                }
                
            }
            
        }

        bool check_win_state(hex_color color){
            vector<bool> visited(121,false);
            if (color == hex_color::RED){
                for(int j = 0; j<11;j++){
                    hex_cell cell = board[0][j];
                    if (cell.get_color() == color){
                        if(dfs(j,color,visited)){
                            return true;
                        }
                    }
                }
            }
            if (color == hex_color::BLUE){
                for(int i = 0; i<11;i++){
                    hex_cell cell = board[i][0];
                    if (cell.get_color() == color){
                        if (dfs(i*11,color,visited)){
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        bool dfs(int node,hex_color color,vector<bool>& visited){
            visited[node] = true;
            if (color == hex_color::RED && node/11 == 10){
                return true;
            }
            else if(color == hex_color::BLUE && node%11 == 10){
                return true;
            }
            
            for(int neighbor:edge_list[node]){
                int n_i = neighbor/11;
                int n_j = neighbor%11;
                hex_cell n_cell = board[n_i][n_j];
                if (!visited[neighbor]&&n_cell.get_color() == color){
                    if (dfs(neighbor,color,visited)){
                        return true;
                    }
                }
            }
            return false;
        }
        


    private:
        vector<vector<int>> edge_list;
        vector<vector<hex_cell>> board;
        void make_cell(int i,int j,vector<int>&neighbors){
            
            vector<int> i_neighbors = {-1, -1, 0, 0, 1, 1};
            vector<int> j_neighbors = { 0,  1,-1, 1,-1, 0};
            for (int k = 0;k<6;k++){
                int i_neigh = i+i_neighbors[k];
                int j_neigh = j+j_neighbors[k];
                if(j_neigh<11 && j_neigh>-1 && i_neigh<11 && i_neigh>-1){
                    int node_num = i_neigh * 11 + j_neigh;
                    neighbors.push_back(node_num);
                }
            }
            
        }
};

int main(){
    hex_graph g;
    g.print_board();
    return 0;
}
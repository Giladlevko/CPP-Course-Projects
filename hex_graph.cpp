#include<iostream>
#include<vector>
#include<iomanip>
using namespace std;

enum class hex_color{NONE,RED,BLUE};

//Name: operator<<
//Input:
// out - output stream object
// c - hex color enum value
//Output:
// Returns the modified output stream
//Description:
// Overloads the output operator for the hex_color enum so
// colors can be printed in a readable format.
ostream& operator<<(ostream& out,const hex_color& c){
    switch(c){
        case hex_color:: RED: out<<"R";break;
        case hex_color:: BLUE: out<<"B";break;
        case hex_color:: NONE: out<<".";break;
        default:out<<"UNKNOWN";break;
    }
    return out;
}


//Name: hex_group
//Input:
// None
//Output:
// Creates a hex_group object
//Description:
// Represents a connected group of hex cells.
// Tracks whether the group touches the
// starting and ending sides needed for a win.
class hex_group{
    public:
        bool touches_start = false;
        bool touches_end = false;
        hex_group* parent = nullptr;
};



class hex_cell{
    public:
    //Name: hex_cell
    //Input:
    // r - row position of the cell
    // column - column position of the cell
    // c - color of the cell
    //Output:
    // Creates a hex_cell object
    //Description:
    // Represents a single hex board cell storing its position,
    // color, and connected group information.
        hex_cell( int r = 0,int colunm = 0, hex_color c = hex_color::NONE):
        row(r),col(colunm),color(c){}

        //Name: set_color
        //Input: c - new color for the cell
        //Output: None
        //Description: 
        // Sets the color of the hex cell.
        void set_color(hex_color c){color = c;}

        //Name: set_color
        //Input: c - new color for the cell
        //Output: None
        //Description:
        // Sets the color of the hex cell.
        hex_color get_color()const{return color;}

        //Name: set_row
        //Input: r - new row value
        //Output: none
        //Description:
        // Sets the row position of the cell.
        void set_row(int r){row = r;}

        //Name: set_col
        //Input:
        // c - new column value
        //Output: none
        //Description:
        // Sets the column position of the cell.
        void set_col(int c){col = c;}

        //Name: get_row
        //Input: none
        //Output: Returns the row position
        //Description:
        // Retrieves the row index of the cell.
        int get_row()const{return row;}

        //Name: get_col
        //Input: none
        //Output: Returns the column position
        //Description: Retrieves the column index of the cell.
        int get_col()const{return col;}

        //Name: get_group
        //Input: none
        //Output: Returns a pointer to the cell's group
        //Description:
        // Retrieves the connected group associated with the cell.
        hex_group* get_group()const{return group;}
        //Name: set_group
        //Input: g - pointer to a hex_group
        //Output: none
        //Description:
        // Assigns the cell to a group.
        void set_group(hex_group* g){group = g;}
    private:
        hex_color color;
        int col;
        int row;
        hex_group* group = nullptr;
};

class hex_graph{
    public:

        //Name: hex_graph
        //Input: none
        //Output: Creates a hex_graph object
        //Description:
        // Builds the Hex game board, initializes all cells,
        // and generates adjacency information for each node.
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
        
        // Name: print_graph
        // Input: none
        //Output: Prints adjacency information
        //Description:
        // Displays every board cell and all neighboring cells
        // connected to it in the graph representation.
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

        //Name: print_board
        //Input: none
        //Output: Prints the game board
        //Description:
        // Displays the current Hex board with coordinates for
        // empty cells and colors for occupied cells.
        void print_board(){
            cout<<"   ";
            for(int j = 0;j<11;j++){
                cout<<j<<"   ";//print colunm index
            }
            cout<<endl;
            for(int i = 0;i<11;i++){
                for (int s = 0;s<i;s++){
                    cout<<"  ";
                }
                if (i<10){cout<<i<<"  ";}//print row index
                else {cout<< i<<" ";}

                for(int j = 0;j<11;j++){
                    cout<<board[i][j].get_color();
                    if (j<10){cout<<" --- ";}
                }
                cout<<endl;
                if (i <10){
                    for(int s = 0; s<i;s++){cout<<"  ";}
                    cout <<"    \\  /";
                    for(int j = 0;j<9;j++){cout<<"  \\  /";}
                    cout<<"  \\";
                    cout<<endl;
                }
            }
            cout<<endl<<endl;
        }


        //Name: find_master_group
        //Input: reference to a hex cell
        //Output: Returns the root group pointer of the cell
        //Description:
        // Traverses parent pointers to find the master group
        // representing the connected component of the cell.
        hex_group* find_master_group(hex_cell& cell){
            hex_group* current = cell.get_group();
            if (current == nullptr) {return nullptr;}
            while(current->parent != nullptr){
                current = current->parent;
            }
            cell.set_group(current);
            return current;
        }


        //Name: is_cell_valid
        //Input: row index, column index
        //Output: Returns true if the cell is within board bounds
        //Description:
        // Checks whether the provided coordinates exist on
        // the Hex board.
        bool is_cell_valid(int i,int j){
            if (i<11 && i>-1 && j<11 && j>-1){
                return true;
            }
            return false;
        }


        //Name: is_cell_empty
        //Input: row index, column index
        //Output: Returns true if the cell is empty
        //Description:
        // Determines whether a valid board position has not
        // yet been occupied by a player.
        bool is_cell_empty(int i,int j){
            if(is_cell_valid(i,j)){
                if (board[i][j].get_color() == hex_color:: NONE){
                 return true;
                }
            }
            return false;
        }


        //Name: check_neighbors_group
        //Input: pointer to the placed cell, color of the current player
        //Output: none
        //Description:
        // Connects the placed cell with neighboring cells of
        // the same color and merges connected groups together.
        // If no group found create a new one.
        // A new group is added to a vector for later deletion.
        void check_nighbors_group(hex_cell* cell,hex_color color){
            int n_num = cell->get_row()*11 + cell->get_col();
            for (int neighbor:edge_list[n_num]){
                hex_group* cell_group = find_master_group(*cell);
                int n_i = neighbor/11;
                int n_j = neighbor%11;
                hex_cell* neighbor_cell = &board[n_i][n_j];
                hex_group* neighbor_group = find_master_group(*neighbor_cell);
                if (neighbor_cell->get_color() == color){
                    if (cell_group == nullptr){
                        cell->set_group(neighbor_group);
                    }
                    else{
                        if (cell_group != neighbor_group){
                            if (neighbor_group->touches_start){cell_group->touches_start = true;}
                            if (neighbor_group->touches_end){cell_group->touches_end = true;}
                            neighbor_group->parent = cell_group;
                        }
                    }
                }
            }
            if (find_master_group(*cell) == nullptr){
                hex_group* new_group = new hex_group;
                cell->set_group(new_group);
                all_hex_groups.push_back(new_group);

            }
        }


        //Name: place_cell
        //Input: row index, column index, color to place
        //Output: none
        //Description:
        // Places a piece on the board, updates group
        // connections, and tracks edge connections for win checks.
        void place_cell(int i,int j,hex_color color){
            hex_cell* cell = &board[i][j];
            cell->set_color(color);

            check_nighbors_group(cell,color);

            hex_group* cell_group = find_master_group(*cell);
            if (color == hex_color::RED){
                if (i == 0){cell_group->touches_start = true;}
                if (i == 10){cell_group->touches_end = true;}
            }
            if (color == hex_color::BLUE){
                if(j==0){cell_group->touches_start = true;}
                if(j==10){cell_group->touches_end = true;}
            }
        }


        //Name: get_player_color
        //Input: player id
        //Output: Returns the player's color
        //Description:
        // Converts a player number into its corresponding hex color.
        hex_color get_player_color(int id){
            hex_color c;
            switch(id){
                case 1: c = hex_color:: BLUE;break;
                case 2: c = hex_color:: RED;break;
                default: cout << "ERROR: player color unknown";break;
            }
            return c;
        }


        //Name: get_input
        //Input: row index, column index - (call by reference)
        //  p_id - current player id
        //Output: none
        //Description:
        // Prompts the player for input until a valid empty
        // board position is selected.
        void get_input(int& i,int &j,int p_id){
            while (!is_cell_valid(i,j)){
                cout<<"Player "<<p_id<< " enter coords of cell:";
                cin >> i >> j;
                if (is_cell_empty(i,j)){
                    place_cell(i,j,get_player_color(p_id));
                    
                }
                else{cout<<"\nPlease enter a valid cell:\n";}
            }
        }


        //Name: win_state
        //Input: row index and column index of last move
        //Output: Returns true if the player has won
        //Description:
        // Determines whether the connected group containing
        // the last placed cell touches both required sides.
        bool win_state(int i,int j){
            hex_cell* cell = &board[i][j];
            hex_group* group = find_master_group(*cell);
            if (group->touches_end && group -> touches_start){
                return true;
            }
            return false;
        }


        //Name: delete_hex_groups
        //Input: none
        //Output: none
        //Description:
        // Frees dynamically allocated group memory and resets
        // all board group pointers.
        void delete_hex_groups(){
            for (hex_group* group:all_hex_groups){
                delete group;
            }
            all_hex_groups.clear();
            for(int i = 0;i < 11;i++){
                for(int j = 0;j < 11;j++){
                    board[i][j].set_group(nullptr);
                }
            }
        }


        //Name: game_loop
        //Input: none
        //Output: none
        //Description:
        // Alternates turns between players until one achieves
        // a winning connection across the board.
        void game_loop(){
            bool game_won = false;
            int player_id = 1;
            cout<<"Player 1 is RED and must connect north to south!\n";
            cout <<"Player 2 is BLUE and must connect east to west\n";
            while(!game_won){
                int i = -1,j = -1;
                print_board();
                get_input(i,j,player_id);
                if (win_state(i,j)){
                    cout << "\nGAME OVER! player "<<player_id<<" wins!\n";
                    print_board();
                    game_won = true;
                    delete_hex_groups();
                }
                else{player_id = ((player_id % 2)+1);}
            }
        }
    
    private:
        vector<vector<int>> edge_list;
        vector<vector<hex_cell>> board;
        vector<hex_group*> all_hex_groups;

        //Name: make_cell
        //Input: row index, column index, 
        // reference to vector storing neighbors
        //Output: none
        //Description:
        // Generates all valid neighboring cells for a board
        // position and stores them in the adjacency list.
        void make_cell(int i,int j,vector<int>&neighbors){
            vector<int> i_neighbors = {-1, -1, 0, 0, 1, 1};
            vector<int> j_neighbors = { 0,  1,-1, 1,-1, 0};
            for (int k = 0;k<6;k++){
                int i_neigh = i+i_neighbors[k];
                int j_neigh = j+j_neighbors[k];
                if(is_cell_valid(i_neigh,j_neigh)){
                    int node_num = i_neigh * 11 + j_neigh;
                    neighbors.push_back(node_num);
                }
            }
            
        }
};


//Name: main
//Input: none
//Output: Returns program exit status
//Description:
// Creates the Hex game object and starts the game loop.
int main(){
    hex_graph g;
    g.game_loop();
    return 0;
}
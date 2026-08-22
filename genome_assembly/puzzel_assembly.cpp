#include<iostream>
#include<vector>
#include<string>

using namespace std;

//pieces are given in the form (top,left,bottom,right) 
//where each side is a color

struct block{
    block(string entry){
        used = false;
        int start = 1;
        int len = 1;
        int entry_end = entry.size();
        int comma_count = 0;
        for(int i = start; i<entry_end; i++){
            if(entry[i] != ',' && i != entry_end-1){continue;}
            len = i-start;
            comma_count++;
            string color = entry.substr(start,len);
            start = i+1;
            switch(comma_count){
                case 1: top = color;break;
                case 2: left = color;break;
                case 3: bottom = color;break;
                case 4: right = color;break;
            }
        }
    }
    block():used(false){}
    void print()const{
        cout<<"("<<top<<","<<left<<","<<bottom<<","<<right<<")";
    }
    string top;
    string bottom;
    string left;
    string right;
    bool used;
};


bool can_place(
    const vector<vector<block>>&board,
    const vector<block>&blocks,
    const block&b, 
    const int& row,const int& col
){
    int last_row = board.size()-1;
    int last_col = board[0].size()-1;
    string border = "black";
    if(row == 0 && b.top != border){return false;}

    if(col == 0 && b.left != border){return false;}

    if(col>0 && (b.left == border || b.left != board[row][col-1].right)){return false;}

    if(row>0 && (b.top == border|| b.top != board[row-1][col].bottom)){return false;}

    if(row == last_row){
        if(b.bottom != border){return false;}
    }
    else if(b.bottom == border){return false;}

    if(col == last_col){
        if(b.right != border){return false;}
    }
    else if(b.right == border){return false;}

    return true;
}

bool solve(
    vector<vector<block>>&board,
    vector<block>&blocks,
    int block_index
){
    

    int row_count = board.size();
    int col_count = board[0].size();
    
    if(block_index == row_count*col_count){return true;}

    int row = block_index/row_count;
    int col = block_index%col_count;

    for(block& b:blocks){
        if(b.used){continue;}
        if(can_place(board,blocks,b,row,col)){
            b.used = true;
            board[row][col] = b;
            if(solve(board,blocks,block_index + 1)){
                return true;
            }
            b.used = false;
        }
    }
    return false;
}

void print(const vector<vector<block>>&board){
    int size = board.size();
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++){
            board[i][j].print();
            if(j<size-1){cout<<";";}
            
        }
        cout<<"\n";
    }
}

void solve_puzzle(vector<block>&blocks){
    //board is 5*5
    int size = 5;
    vector<vector<block>>board(size,vector<block>(size));
    int start = 0;
    if(solve(board,blocks,start)){
        print(board);
    }
    else{cout<<"Impossible";}
}

int main(){
    int count = 25;//5*5
    vector<block>blocks(count);
    for(int i = 0; i<count; i++){
        string entry;
        cin>>entry;
        blocks[i] = block(entry);
    }
    solve_puzzle(blocks);
}
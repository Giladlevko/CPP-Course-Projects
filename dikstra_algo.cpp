#include <iostream>
#include <ctime>
#include <cstdio>

using namespace std;

class graph{
    public:
        double x,y;
        vector<graph>neighbors;//literlly no clue what im doing
        graph find_closest_neighbor(){
            for (i=0;neighbors.size();++i){

            }
        }

}

double prob(){
    return static_cast<double>(rand())%1
}

//creates graph matrix and assignes edges randomly
bool** graph{
    srand(time(0));
    graph = new bool*[size];
    for(int i = 0;i<size;++i){
        graph[i] = new bool[size];
    }
    for(int i = 0; i < size; ++i){
        for (int j = i; j < size; ++j){
            if (i == j){graph[i][j] = false;}
            //checks only the upper indexes 
            //so it cant be reset by a later iteration
            else{ graph[i][j] = graph[j][i] = (prob()<0.19);}
        }
    }
}


bool is_connected(bool *graph[],int size){
    int old_size = 0,c_size = 0;
    bool* close = new bool[size];
    bool* open = new bool[size];
    for(int i = 0; i < size;++i){
        open[i] = close[i] = false;
    }
    open[0] = true;

    //add to close
    while(c_size < size){
        for(int i = 0; i<size; ++i){
            old_size = c_size;
            if(open[i] && (close[i] == false)){
                close[i] = true;
                c_size++;
                //add to open
                for (int j = 0; j<size;++j){
                    open[j] = open[j] || graph[i][j];
                }
            }

        }
        if(c_size == size)return true;
        if (old_size == c_size) return false;
    }
}

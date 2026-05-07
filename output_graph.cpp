#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
using namespace std;

double prob(){
    return(static_cast<double>(rand())/RAND_MAX);
}

void gen_rand_graph(bool**& graph, int**& color, int**& cost,int size, double density){
    for (int i = 0; i< size;i++){
        for (int j = i;j<size;j++){
            if (i==j){graph[i][j] = false;}
            else{
                graph[i][j] = graph[j][i] =(prob()<density);
            }
        }
    }
    for(int i = 0; i<size; i++){
        for (int j = i;j<size;j++){
            if (graph[i][j]){
                color[i][j] = color[j][i] = rand()%3;
                cost[i][j] = cost[j][i] = prob()*30;

            }
        }
    }
}

void construct_graph(bool**& graph, int**& color, int**& cost,int size){
    graph = new bool*[size];
    color = new int*[size];
    cost = new int*[size];
    for(int i = 0; i<size; i++){
        graph[i] = new bool[size];
        color[i] = new int[size];
        cost[i] = new int[size];
    }
    
}

void print_to_file(bool** graph, int** color, int** cost,int size){
    ofstream outp("graph_data.txt");
    if (!outp){
        cout << "Failed to open file\n";
        return;
    }
    outp << size << endl;
    for (int i = 0; i<size;i++){
        for (int j = 0; j<size;j++){
            if (graph[i][j]){
                outp << i<<": "<<j<<": cost: "<<cost[i][j]<<": color: "<<color[i][j]<<endl;
            }
        }
    }
}

void delete_graph(bool** graph, int** color, int** cost,int size){
    for (int i = 0; i<size; i++){
        delete[] graph[i];
        delete[] cost[i];
        delete[] color[i];
    }
    delete[] graph;
    delete[] cost;
    delete[] color;
}


int main(void){
    int size = 15;
    double density;
    cout<<"Graph size?"<<endl;
    cin >> size;
    cout<<"Graph density(0,1)?"<<endl;
    cin >> density;
    bool** graph;
    int** color;
    int** cost;
    srand(time(0));
    construct_graph(graph,color,cost,size);
    gen_rand_graph(graph,color,cost,size,density);
    print_to_file(graph,color,cost,size);
    delete_graph(graph,color,cost,size);
    


}
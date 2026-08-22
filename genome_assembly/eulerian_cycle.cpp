#include<iostream>
#include<vector>
#include<list>
#include<algorithm>

using namespace std;


//I used this video by William Fiset for help with the Algorithm
//And I found it very useful to understand how it works!
//https://youtu.be/8MpoO2zA2l4?si=YY6ryUyZVQJ45GRo


void count_in_out_degree(
    const vector<vector<int>>&graph,
    vector<int>&in_count,vector<int>&out_count
){
    int size = graph.size();
    for(int i = 0; i<size; i++){
        for(const int&v:graph[i]){
            in_count[v]++;
            out_count[i]++;
        }
    }
}

bool graph_has_eulerian_path(
    const vector<int>&in_count, 
    const vector<int>&out_count
){
    int size = in_count.size();
    int start_nodes = 0, end_nodes = 0;
    for(int i = 0; i<size; i++){
        //for a path to exist there must be at most
        // a difference of 1 between the in / out degrees
        if ( abs(in_count[i] - out_count[i]) > 1){
            return false;
        }
        else if(out_count[i] - in_count[i] == 1){
            start_nodes++;

        }

        else if(in_count[i] - out_count[i] == 1){
            end_nodes++;
        }

    }
    //either there are no start/end nodes or there are 1 of each
    bool has_path = (start_nodes == end_nodes == 0) || (start_nodes == end_nodes == 1);


    return has_path;
}


int get_start_node(
    const vector<int>&in_count, 
    const vector<int>&out_count
){
    int size = in_count.size();
    int start = 0;
    for(int i = 0; i<size; i++){
        //if a vert has more out than in by 1
        //it is a unique starting node
        if((out_count[i] - in_count[i]) == 1){return i;}

        //otherwise just choose some node with an out degree
        //of at least one
        if(out_count[i]>0){
            start = i;
        }
    }
    return start;
}


void dfs(
    const vector<vector<int>>&graph,
    list<int>&path,int curr,
    vector<int>&out_count
){
    while(out_count[curr] > 0){
        //select the next unvisited edge
        //the out_count is used bothe to know how much
        //edges I have left to discover 
        //and also to index the next node 
        int next_node_index = --out_count[curr];
        int next_node = graph[curr][next_node_index];

        dfs(graph,path,next_node,out_count);

    }
    path.push_front(curr);
}


list<int> get_eulerian_path(
    const int& vert_count, const int& edge_count,
    vector<vector<int>>&graph
){
    list<int>path;
    vector<int>in_count(vert_count,0);
    vector<int>out_count(vert_count,0);
    count_in_out_degree(graph,in_count,out_count);
    if( !graph_has_eulerian_path(in_count,out_count) ){
        return path;
    }
    int start_node = get_start_node(in_count,out_count);

    dfs(graph,path,start_node,out_count);

    //if we didnt traverse a correct number of verices
    //i.e edge_count+1 than our path doesnt exist
    //therefore we should return an empty one;
    if(path.size() != edge_count+1){path.clear();}

    return path;
}

void print_cycle(const list<int>&path){
    //a cycle must end in the same verex
    if(path.empty() || path.back() != path.front()){
        cout<<"0";
        return;
    }
    cout<<"1\n";

    //no need to output the last element
    //since it is already the start element

    for(auto it = path.begin(); it != prev(path.end()); it++){
        //output the vert but return to 1 index
        cout<<*it+1<<" ";
    }
}

int main(){
    int vert_count,edge_count;
    cin>>vert_count>>edge_count;
    vector<vector<int>>graph(vert_count);
    for(int i = 0; i<edge_count; i++){
        int u,v;
        cin>>u>>v;
        //transform to 0 index
        u--,v--;
        graph[u].push_back(v);
    }
    list<int> path = get_eulerian_path(vert_count,edge_count,graph);
    print_cycle(path);
}
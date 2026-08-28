#include<iostream>
#include<vector>
#include<list>
#include<string>
#include<unordered_map>
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
        //the out_count is used both to know how much
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

void print_cycle(const list<int>&path,const vector<string>&id_to_str){
    //a cycle must end in the same verex
    if(path.empty() || path.back() != path.front()){
        cout<<"0";
        return;
    }

    //no need to output the last element
    //since it is already the start element

    for(auto it = path.begin(); it != prev(path.end()); it++){
        //we only need to output the last char because that is what changed
        //since the vertex before it or the last vertex if its the first
        char last_char = id_to_str[*it].back();
        cout<<last_char;
    }
}



int get_id(
    const string& s,
    unordered_map<string,int>& str_to_id,
    vector<string>& id_to_str,
    vector<vector<int>>& graph
){
    auto it = str_to_id.find(s);
    if(it != str_to_id.end()){
        return it->second;
    }
    int id = id_to_str.size();
    id_to_str.push_back(s);
    str_to_id[s] = id;
    graph.push_back({});
    return id;
}



vector<vector<int>> create_k_mer_graph(const vector<string>& entries,vector<string>& id_to_str){
    int count = entries.size();
    vector<vector<int>> graph;
    //all entries are the same length
    int k = entries[0].size();
    unordered_map<string,int> str_to_id;
    for(int i = 0; i<count; i++){
        //vert size is k-1 so the edge is k long
        string pre = entries[i].substr(0,k-1);
        string suff = entries[i].substr(1,k-1);
        int u = get_id(pre,str_to_id,id_to_str,graph);
        int v = get_id(suff,str_to_id,id_to_str,graph);
        graph[u].push_back(v);
    }
    return graph;
}


int main(){
    //it is given that the k-mer count is 5396
    int count = 5396;
    vector<string>entries(count);
    for(int i = 0; i<count; i++){
        cin>>entries[i];
    }
    vector<string>id_to_str;
    vector<vector<int>> graph = create_k_mer_graph(entries,id_to_str);
    int edge_count = count;
    int vert_count = id_to_str.size();
    list<int> path = get_eulerian_path(vert_count,edge_count,graph);
    print_cycle(path,id_to_str);
}

#include<vector>
#include<string>
#include<unordered_map>
#include<algorithm>
#include<iostream>

using namespace std;


//I used this video by William Fiset for help with the Algorithm
//And I found it very useful to understand how it works!
//The video is genrally on Eulerian paths but I used some of the code from there
//https://youtu.be/8MpoO2zA2l4?si=YY6ryUyZVQJ45GRo

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



vector<vector<int>> create_k_mer_graph(const vector<string>& entries,const int& k){
    int count = entries.size();
    vector<string> id_to_str;
    vector<vector<int>> graph;
    unordered_map<string,int> str_to_id;
    for(const string& e:entries){
        //sliding a window through the entry to get all k-mers
        for(int i = 0; i<=e.size()-k; i++){
            //vert size is k-1 so the edge is k long
            string pre = e.substr(i,k-1);
            string suff = e.substr(i+1,k-1);
            int u = get_id(pre,str_to_id,id_to_str,graph);
            int v = get_id(suff,str_to_id,id_to_str,graph);
            //prevent the same edge from being created twice
            if(find(graph[u].begin(),graph[u].end(),v) == graph[u].end()){
                graph[u].push_back(v);
            }
        }
    }
    return graph;
}


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


bool graph_has_unique_eulerian(const vector<vector<int>>&graph){
    if(graph.empty()){return false;}
    int vert_count = graph.size();
    vector<int>in_count(vert_count,0),out_count(vert_count,0);
    count_in_out_degree(graph,in_count,out_count);
    for(int i = 0; i<vert_count; i++){
        //for a unique cycle to exist there 
        //must be exactly 1 in and 1 out edge for each vertex
        if(!(in_count[i] == out_count[i] && out_count[i] == 1)){
            return false;
        }
    }
    
    int start = 0;
    int curr = start;
    int visited_count = 0;
    while(true){
        //by now we know there is one out edge so we can safely choose at 0
        //we will try to traverse the graph and if we reach the start we have a loop
        //to know if this loop is the full graph we check if the visited == vert_count
        //if all of these are true then we have a unique eulerian cycle
        curr = graph[curr][0];
        visited_count++;
        if(curr==start){break;}
    }
    return (visited_count == vert_count);
}


int find_optimal_k(const vector<string>&entries, const int& max_k){
    vector<vector<int>>graph;
    for(int k = max_k-1; k>1; k--){
        graph = create_k_mer_graph(entries,k);
        if(graph_has_unique_eulerian(graph)){return k;}
    }
    return -1;
}

int main(){
    //it is given that there are 400 entries of size 100
    int count = 400;
    int entry_size = 100;
    vector<string>entries(count);
    for(int i = 0; i<count; i++){
        cin>>entries[i];
    }
    cout<<find_optimal_k(entries,entry_size);
}


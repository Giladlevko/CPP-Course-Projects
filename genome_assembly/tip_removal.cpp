#include<vector>
#include<string>
#include<unordered_map>
#include<algorithm>
#include<unordered_set>
#include<queue>
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

            //cout<<u<<"->"<<v<<" = "<<pre<<"->"<<suff<<"\n";

            //prevent the same edge from being created twice
            if(find(graph[u].begin(),graph[u].end(),v) == graph[u].end()){
                graph[u].push_back(v);
            }
        }
    }
    return graph;
}


vector<vector<int>> create_reverse_graph(const vector<vector<int>>& graph){
    int vert_count = graph.size();
    vector<vector<int>> r_graph(vert_count);
    for(int v = 0; v<vert_count; v++){
        for(const int& u:graph[v]){
            r_graph[u].push_back(v);
        }
    }
    return r_graph;
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

queue<int> find_tip_candidates(const vector<vector<int>>&graph){
    queue<int>tips;
    int vert_count = graph.size();
    for(int i = 0; i<vert_count; i++){
        if(graph[i].empty()){
            tips.push(i);
        }
    }
    return tips;
}

void remove_edge(vector<vector<int>>&graph,int u,int v){
    auto it = find(graph[u].begin(),graph[u].end(),v);
    if(it != graph[u].end()){
        graph[u].erase(it);
    }
}

void remove_tip_path(
    int curr,
    vector<int>& in,vector<int>&out,
    vector<vector<int>>&graph,vector<vector<int>>&r_graph,
    queue<int>&tips,
    int& tips_removed,
    const int& max_tip_size
){
    //since the graph changes I need to make sure
    //that what I had in the queue is still valid
    if(in[curr] !=0 || out[curr] == 0){return;}

    vector<int> path;

    //cout<<"curr path: ";

    path.push_back(curr);
    while(out[curr] == 1 && in[curr] <= 1 && path.size()<=max_tip_size+1){
        path.push_back(graph[curr][0]);

        //cout<<curr<<" ";

        curr = graph[curr][0];
    }

    //cout<<"\n";

    int edge_count = path.size()-1;
    if(edge_count>0 && edge_count <= max_tip_size && (in[curr] > 1 || out[curr] > 1 || out[curr] == 0)){
        for(int i = 0; i<edge_count; i++){
            //remove edge u->v
            int u = path[i];
            int v = path[i+1];
            remove_edge(graph,u,v);
            remove_edge(r_graph,v,u);
            in[v]--;
            out[u]--;
            tips_removed++;

            //cout<<u<<"->"<<v<<" removed\n";

            //add new tip to queue if the removal of the current one made another
            if(in[v] == 0 && out[v] > 0){
                tips.push(v);
            }

        }
    }
}

int remove_tips(vector<vector<int>>&graph, const int& max_tip_size){
    int vert_count = graph.size();
    vector<vector<int>> r_graph = create_reverse_graph(graph);
    vector<int> in(vert_count,0),out(vert_count,0);
    vector<int>& r_in = out;
    vector<int>& r_out = in;
    count_in_out_degree(graph,in,out);

    queue<int>tips = find_tip_candidates(r_graph);
    queue<int> r_tips = find_tip_candidates(graph);

    int tips_removed = 0;

    while( !r_tips.empty() || !tips.empty() ){

        if(!tips.empty()){
            int tip = tips.front();
            tips.pop();
            remove_tip_path(tip,in,out,graph,r_graph,tips,tips_removed,max_tip_size);
        }
        if(!r_tips.empty()){
            int tip = r_tips.front();
            r_tips.pop();

            //cout<<"backward tip: "<<tip<<"\n";

            remove_tip_path(tip,r_in,r_out,r_graph,graph,r_tips,tips_removed,max_tip_size);
        }
    }
    return tips_removed;
}

int main(){
    //it is given that k = 15
    int k = 15;
    vector<string>entries;
    string entry;
    while(cin>>entry && entry != "stop"){
        entries.push_back(entry);
    }
    vector<vector<int>>graph = create_k_mer_graph(entries,k);
    cout<<remove_tips(graph,k);
}



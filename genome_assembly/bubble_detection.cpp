#include<vector>
#include<string>
#include<unordered_map>
#include<algorithm>
#include<unordered_set>
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

void record_valid_bubble_vertices_cannidates(
    vector<int>&in_count,vector<int>&out_count,
    unordered_set<int>&in_cannidates,unordered_set<int>&out_cannidates
){
    int size = in_count.size();
    for(int v = 0; v<size; v++){
        if(in_count[v]>=2){
            in_cannidates.insert(v);
        }
        if(out_count[v]>=2){
            out_cannidates.insert(v);
        }
    }
}

bool disjoint_paths(const vector<int>&a,const vector<int>&b,int vert_count){
    vector<bool>visited(vert_count,false);
    int a_size = a.size();
    int b_size = b.size();
    for(const int& u:a){
        visited[u] = true;
    }
    //remark the shared v and w as false
    visited[a[0]] = visited[a.back()] = false;
    for(const int u:b){
        if(visited[u]){return false;}
    }
    return true;
}

void find_paths(
    const vector<vector<int>>&graph,
    const int& curr,
    vector<int>&path,
    vector<bool>&visited,
    unordered_set<int>&in_cannidates,
    unordered_map<int,vector<vector<int>>>& to_w_paths,
    const int& max_depth
){
    //depth is in edges count not vertex count
    int depth = path.size()-1;
    auto it = in_cannidates.find(curr);
    if(depth>0 &&  it != in_cannidates.end()){
        to_w_paths[curr].push_back(path);
        //we dont return here because we might hit another w later
        //before reaching the depth limit
    }

    if(depth == max_depth){return;}

    for(int u:graph[curr]){
        if(visited[u]){continue;}
        visited[u] = true;
        path.push_back(u);

        find_paths(graph,u,path,visited,in_cannidates,to_w_paths,max_depth);

        //we track back here so we could use this
        //vertex for another path to some other w
        path.pop_back();
        visited[u] = false;
    }
}

int count_candidate_bubble_paths(
    const vector<vector<int>>&graph,
    unordered_set<int>&in_cannidates,unordered_set<int>&out_cannidates,
    const int& max_depth
){
    int bubble_count = 0;
    int vert_count = graph.size();
    
    for(const int& v:out_cannidates){
        //keeps track af all paths from v to some w (an in_candidate)
        unordered_map<int,vector<vector<int>>> to_w_paths;

        vector<bool>visited(vert_count,false);
        visited[v] = true;
        vector<int>path = {v};
       
        //records all paths from v to w in the to_w_paths
        find_paths(graph,v,path,visited,in_cannidates,to_w_paths,max_depth);

        //for each pair of paths from v to some w we check
        //if all the inward nodes are different from each other
        //(if the two paths are disjoint) if they are then a bubble exist
        //so we count it
        for(const auto& w_paths:to_w_paths){
            //w_paths.second is the paths from v to w
            int path_count = w_paths.second.size();
            for(int i = 0; i<path_count; i++){
                for(int j = i+1; j<path_count; j++){
                    if(disjoint_paths(w_paths.second[i],w_paths.second[j],vert_count)){
                        bubble_count++;
                    }
                }
            }
        }
    }
    return bubble_count;
}

int get_bubble_count(const vector<vector<int>>&graph,const int& max_depth){
    int vert_count = graph.size();
    vector<int>in_count(vert_count,0),out_count(vert_count,0);
    count_in_out_degree(graph,in_count,out_count);
    unordered_set<int>in_cannidates,out_cannidates;
    record_valid_bubble_vertices_cannidates(in_count,out_count,in_cannidates,out_cannidates);
    return count_candidate_bubble_paths(graph,in_cannidates,out_cannidates,max_depth);
}

int main(){
    int k,max_depth;
    cin>>k>>max_depth;
    vector<string> entries;
    string read;
    while(cin>>read){
        entries.push_back(read);
    }
    vector<vector<int>> graph = create_k_mer_graph(entries,k);
    cout<<get_bubble_count(graph,max_depth);
}
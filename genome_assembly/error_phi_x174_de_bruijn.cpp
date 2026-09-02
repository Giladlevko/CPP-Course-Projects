#include<vector>
#include<string>
#include<unordered_map>
#include<map>
#include<algorithm>
#include<unordered_set>
#include<utility>
#include<queue>
#include<list>
#include<iostream>

using namespace std;


//I used this video by William Fiset for help with the Algorithm
//And I found it very useful to understand how it works!
//https://youtu.be/8MpoO2zA2l4?si=YY6ryUyZVQJ45GRo

//this uses concepts such as bubbles and tips to remove the errors!
//Remove all tips and for each bubble only keep the path of the highest weight
//meaning the path that uses the most agreed upon k-mers

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



vector<vector<int>> create_k_mer_graph(
    const vector<string>& entries,const int& k,
    vector<string>& id_to_str,
    map<pair<int,int>,int>& edge_weight
){
    int count = entries.size();
    
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
            //map auto initializes missing keys to 0 on creation
            //so I can just increment it even if it doesnt exist yet
            edge_weight[{u,v}]++;
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


double get_path_weight(const vector<int>&path,const map<pair<int,int>,int>& edge_weight){
    int size = path.size();
    int edge_count = size-1;
    double total_weight = 0.0;
    for(int i = 0; i<size-1; i++){
        auto it = edge_weight.find({path[i],path[i+1]});
        if(it != edge_weight.end()){
            total_weight += it->second;
        }
    }
    return total_weight / edge_count;
}


void remove_path(const vector<int>&path, vector<vector<int>>&graph){
    int size = path.size();
    for(int i = 0; i< size-1; i++){
        const int& u = path[i];
        const int& v = path[i+1];
        auto it = find(graph[u].begin(),graph[u].end(),v);
        if(it != graph[u].end()){
            graph[u].erase(it);
        }
    }
}


int pop_valid_bubbles(
    vector<vector<int>>&graph,
    unordered_set<int>&in_cannidates,unordered_set<int>&out_cannidates,
    const int& max_depth,
    const map<pair<int,int>,int>& edge_weight
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
                        double weight_i = get_path_weight(w_paths.second[i],edge_weight);
                        double weight_j = get_path_weight(w_paths.second[j],edge_weight);
                        const vector<int>&path_to_remove = weight_i > weight_j ? w_paths.second[j] : w_paths.second[i];
                        
                        //cout<<"bubble path removed:\n";
                        //for(const int&v:path_to_remove){
                        //    cout<<v<<" ";
                        //}
                        //cout<<"\n";

                        remove_path(path_to_remove,graph);
                        bubble_count++;
                    }
                }
            }
        }
    }
    return bubble_count;
}

int remove_bubbles(vector<vector<int>>&graph,const int& max_depth,const map<pair<int,int>,int>& edge_weight){
    int vert_count = graph.size();
    vector<int>in_count(vert_count,0),out_count(vert_count,0);
    count_in_out_degree(graph,in_count,out_count);
    unordered_set<int>in_cannidates,out_cannidates;
    record_valid_bubble_vertices_cannidates(in_count,out_count,in_cannidates,out_cannidates);
    return pop_valid_bubbles(graph,in_cannidates,out_cannidates,max_depth,edge_weight);
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
    bool has_path = (start_nodes == 0 && end_nodes == 0) || (start_nodes == end_nodes == 1);


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


int get_edge_count(vector<vector<int>>&graph){
    int count = 0;
    for(int i = 0; i<graph.size(); i++){
        count += graph[i].size();
    }
    return count;
}

void sequence_genome(
    vector<vector<int>>&graph,const int& k,
    const map<pair<int,int>,int>&edge_weight,
    const vector<string>&id_to_str
){
    //need to remove tips first to not mistake them with bubbles
    remove_tips(graph,k);
    remove_bubbles(graph,k,edge_weight);

    int vert_count = graph.size();
    int edge_count = get_edge_count(graph);
    list<int> eulerian_path = get_eulerian_path(vert_count,edge_count,graph);
    print_cycle(eulerian_path,id_to_str);
}


int main(){
    //it is given that k = 15 but in the discussion people said to use k = 20
    int k = 20;
    vector<string>entries;
    string entry;
    while(cin>>entry && entry != "stop"){
        entries.push_back(entry);
    }
    vector<string>id_to_str;
    map<pair<int,int>,int>edge_weight;
    vector<vector<int>>graph = create_k_mer_graph(entries,k,id_to_str,edge_weight);
    sequence_genome(graph,k,edge_weight,id_to_str);

}

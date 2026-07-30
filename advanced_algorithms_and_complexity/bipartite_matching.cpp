#include<vector>
#include<queue>
#include<iostream>
#include<algorithm>
using namespace std;

void update_residual(
    vector<vector<long long>>& graph, 
    const vector<int>& parent,
    const int& s,const int& t,
    const long long& flow
){
    int curr = t;
    while(curr != s){
        int child = curr;
        curr = parent[curr];
        graph[curr][child] -= flow;
        graph[child][curr] += flow;
    }
}



bool bfs_has_path(
    vector<vector<long long>>& graph,
    vector<int>& parents,
    const int& s, const int& t
){ 
    fill(parents.begin(),parents.end(),-1);
    parents[s] = s;
    queue<int> q;
    q.push(s);
    while(!q.empty()){
        int u = q.front();
        q.pop();
        for(int v = 1; v<graph.size();v++){
            if(parents[v] == -1 && graph[u][v]>0){
                q.push(v);
                parents[v] = u;
                if(v==t){return true;}
            }
        }
    }
    return (parents[t] != -1);
}

long long find_min_flow(
    const vector<vector<long long>>&graph,
    const vector<int>&parent,
    const int& s,const int& t
){
    int curr = t;
    long long min_flow = 1e18;
    while(curr != s){
        int child = curr;
        curr = parent[curr];
        long long capacity = graph[curr][child];
        min_flow = min(min_flow,capacity);
    }
    return min_flow;
}

int get_max_flow(
    vector<vector<long long>>& graph,
    const int& s, const int& t
){
    vector<int>parent(graph.size(),-1);
    int flow = 0;
    while(bfs_has_path(graph,parent,s,t)){
        //gets the smallest flow in the path from s to t
        // that way we know what the bottle-neck is
        long long min_flow = find_min_flow(graph,parent,s,t);
        update_residual(graph,parent,s,t,min_flow);
        flow +=min_flow;
    }
    return flow;
}


void bipartite_matching(
    vector<vector<long long>>&graph,
    const vector<vector<int>>& potential_matches,
    const int& nodes_count,
    const int& s,const int& t
){
    get_max_flow(graph,s,t);
    int first_node = 2;
    for(int i = 0;i<nodes_count;i++){
        int node_index = i+first_node;
        int match_index = -1;
        for(const int& task_index:potential_matches[node_index]){
            int task_node = task_index + nodes_count +2;
            if(graph[node_index][task_node] == 0){
                match_index = task_index +1;
                break;
            }
        }
        cout<<match_index<<" ";
    } 
}


int main(){
    int nodes,tasks;
    cin>>nodes>>tasks;
    int size = nodes+tasks+2;
    int s = 1,t = size;
    vector<vector<long long>> graph(size+1,vector<long long>(size+1,0));
    vector<vector<int>>potential_matches(nodes+2);
    for(int i = 0;i<nodes;i++){
        int from = i+2;
        graph[s][from] = 1;
        for(int j = 0;j<tasks;j++){
            int a;
            cin>>a;
            if( a == 1){
                int to = j+nodes+2;
                graph[from][to] = a;
                potential_matches[i+2].push_back(j);
            }

        }
    }
    for(int i = 0;i<tasks;i++){
        int task_index = i+nodes+2;
        graph[task_index][t] = 1;
    }
    bipartite_matching(graph,potential_matches,nodes,s,t);
}
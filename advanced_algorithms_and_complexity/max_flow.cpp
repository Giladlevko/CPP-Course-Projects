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

int main(){
    int nodes,edges;
    cin>>nodes>>edges;
    vector<vector<long long>> graph(nodes+1,vector<long long>(nodes+1,0));
    for(int i = 0; i<edges;i++){
        int a,b,c;
        cin>>a>>b>>c;
        graph[a][b] += c;
    }
    int s = 1,t = nodes;
    cout<<get_max_flow(graph,s,t);
}
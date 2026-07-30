#include<vector>
#include<queue>
#include<iostream>
#include<algorithm>
using namespace std;


//Gets data on different stock graphs and finds 
// how many charts are needed to represent them without collision

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

bool graph_bigger(const vector<int>&a,const vector<int>&b){
    int size = a.size();
    for(int i = 0; i<size; i++){
        if(a[i] <= b[i]){
            return false;
        }
    }
    return true;
}

vector<vector<long long>> build_bipartite_from_stock_data(vector<vector<int>>stocks){
    int stock_count = stocks.size();
    int size = 2* stock_count + 2;
    vector<vector<long long>> graph(size+1,vector<long long>(size+1,0));
    int bigger_group_start = 2;
    int smaller_group_start = bigger_group_start + stock_count;
    for(int i = 0;i<stock_count;i++){
        for(int j = 0;j<stock_count;j++){
            if(j==i){continue;}
            if(graph_bigger(stocks[i],stocks[j])){
                graph[i+bigger_group_start][j+smaller_group_start] = 1;
            }
        }
    }
    int s = 1,t = size;
    for(int i = bigger_group_start;i<smaller_group_start;i++){
        graph[s][i] = 1;
    }
    for(int i = smaller_group_start;i<size;i++){
        graph[i][t] = 1;
    }
    return graph;
}


int main(){
    int stock_count,points;
    cin>>stock_count>>points;
    vector<vector<int>>stocks(stock_count,vector<int>(points,0));
    for(int i = 0; i<stock_count; i++){
        for(int j = 0; j<points; j++){
            int a;
            cin>>a;
            stocks[i][j] = a;
        }
    }
    vector<vector<long long>> graph = build_bipartite_from_stock_data(stocks);
    int s = 1,t = 2*stock_count+2;
    int matches = get_max_flow(graph,s,t);
    int chart_count = stock_count - matches;
    cout<<chart_count;
}
#include<vector>
#include<iostream>
#include<algorithm>
using namespace std;

void explore(
    const int& v_index,vector<bool>&visited,
    const vector<vector<int>>&edges,vector<int>&post_order
){
    visited[v_index] = true;
    for(const int& i:edges[v_index]){
        if (!visited[i]){
            explore(i,visited,edges,post_order);
        }
    }
    post_order.push_back(v_index);
}


void dfs(const vector<vector<int>>&edges,vector<int>&post_order){
    int vert_count = edges.size();
    vector<bool>visited(vert_count,false);
    for(int v = 1;v<vert_count;v++){
        if(!visited[v]){
            explore(v,visited,edges,post_order);
        }
    }
}

vector<vector<int>> reverse_graph(const vector<vector<int>> edges){
    int vert_count = edges.size();
    vector<vector<int>> reversed(vert_count);
    for(int i = 1;i<vert_count;i++){
        for(const int& j:edges[i]){
            reversed[j].push_back(i);
        }
    }
    return reversed;
}

int scc_count(const vector<vector<int>>&edges){
    int vert_count = edges.size();

    vector<vector<int>> reversed_edges = reverse_graph(edges);
    vector<int> reversed_post_order;
    dfs(reversed_edges,reversed_post_order);

    int scc_amount = 0;

    vector<bool>visited(vert_count,false);
    vector<int> post_order;
    vector<bool>in_scc(vert_count,false);
    vector<int>reversed_order;
    for(int v = reversed_post_order.size()-1;v>=0;v--){
        int vertex = reversed_post_order[v];
        if(!visited[vertex]){
            explore(vertex,visited,edges,post_order);
            scc_amount++;

        }
    }
    return scc_amount;
}


int main(){
    int e,v;
    cin>>v>>e;
    vector<bool> visited(v+1,false);
    vector<int>post_order;
    vector<vector<int>>edges(v+1);
    for(int i = 0;i<e;i++){
        int a,b;
        cin>>a>>b;
        edges[a].push_back(b);
    }
    cout<<scc_count(edges);
}
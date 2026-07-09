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


void dfs(vector<bool>&visited,const vector<vector<int>>&edges,vector<int>&post_order){
    int vert_count = edges.size();
    for(int v = 1;v<vert_count;v++){
        if(!visited[v]){
            explore(v,visited,edges,post_order);
        }
    }
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
    dfs(visited,edges,post_order);
    for(int i=v-1;i>=0;i--){
        cout<<post_order[i]<<" ";
    }
}
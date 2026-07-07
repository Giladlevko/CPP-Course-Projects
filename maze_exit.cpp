#include<vector>
#include<iostream>
using namespace std;

void explore(const int& v_index,vector<bool>&visited,const vector<vector<int>>&edges,vector<int>&groups,const int& group){
    visited[v_index] = true;
    groups[v_index] = group;
    for(const int& i:edges[v_index]){
        if (!visited[i]){
            explore(i,visited,edges,groups,group);
        }
    }
}

void dfs(vector<bool>&visited,const vector<vector<int>>&edges,vector<int>&groups){
    int group = 1;
    int vert_count = edges.size();
    for(int v = 1;v<vert_count;v++){
        if(!visited[v]){
            explore(v,visited,edges,groups,group);
            group++;
        }
    }
}

int main(){
    int e,v;
    cin>>v>>e;
    vector<bool> visited(v+1,false);
    vector<vector<int>>edges(v+1);
    vector<int>groups(v+1);
    for(int i = 0;i<e;i++){
        int a,b;
        cin>>a>>b;
        edges[a].push_back(b);
        edges[b].push_back(a);
    }
    dfs(visited,edges,groups);
    int v1,v2;
    cin>>v1>>v2;
    cout<<(groups[v1]==groups[v2]);
}
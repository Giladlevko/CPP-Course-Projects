#include<vector>
#include<queue>
#include<iostream>
using namespace std;

void bfs(const vector<vector<int>>& edges,const int& start_node,vector<int>&dist){
    int vert_count = edges.size();
    for(int i = 1;i<vert_count;i++){
        dist[i] = -1;//-1 means unreachable
    }
    dist[start_node] = 0;
    queue<int> q;
    q.push(start_node);
    int curr;
    while(!q.empty()){
        curr = q.front();
        q.pop();
        for(const int& v:edges[curr]){
            if(dist[v] == -1){
                q.push(v);
                dist[v] = dist[curr]+1;
            }
        }
    }
}

int main(){
    int e,v;
    cin>>v>>e;
    vector<int> dist(v+1);
    vector<vector<int>>edges(v+1);
    for(int i = 0;i<e;i++){
        int a,b;
        cin>>a>>b;
        edges[a].push_back(b);
        edges[b].push_back(a);
    }
    int start_node,end_node;
    cin>>start_node>>end_node;
    bfs(edges,start_node,dist);
    cout<<dist[end_node];
}
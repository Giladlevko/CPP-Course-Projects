#include<vector>
#include<iostream>
#include<algorithm>
using namespace std;

void explore(
    const int& v_index,vector<bool>&visited,
    const vector<vector<int>>&edges,vector<bool>&is_visiting,bool&g_has_cycle
){
    visited[v_index] = true;
    is_visiting[v_index] = true;
    for(const int& i:edges[v_index]){
        if (!visited[i]){
            explore(i,visited,edges,is_visiting,g_has_cycle);
        }
        else if(is_visiting[i]){
            g_has_cycle = true;
            return;
        }
        if(g_has_cycle){return;}
    }
    is_visiting[v_index] = false;
}

void has_cycle(bool has){
    cout<<has;
}

void dfs(vector<bool>&visited,const vector<vector<int>>&edges){
    int vert_count = edges.size();
    vector<bool>is_visiting(vert_count,false);
    bool g_has_cycle = false;
    
    for(int v = 1;v<vert_count;v++){
        if(!visited[v]){
            explore(v,visited,edges,is_visiting,g_has_cycle);
        }
        else if(is_visiting[v]){
            g_has_cycle = true;
        }
        if(g_has_cycle){
            has_cycle(true);
            return;
        }
    }
    has_cycle(false);
}


int main(){
    int e,v;
    cin>>v>>e;
    vector<bool> visited(v+1,false);
    vector<vector<int>>edges(v+1);
    for(int i = 0;i<e;i++){
        int a,b;
        cin>>a>>b;
        edges[a].push_back(b);
    }
    dfs(visited,edges);
}

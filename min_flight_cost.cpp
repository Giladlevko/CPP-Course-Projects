#include<vector>
#include<iostream>
#include<climits>

using namespace std;
const int INF = INT_MAX;

struct edge{
    edge(int t,long long c):to(t),cost(c){}
    int to;
    long long cost;
};

void relax(const int& u,const int& v,vector<long long>& dist,const edge& e){
    if (dist[v]>dist[u]+e.cost){
        dist[v]=dist[u]+e.cost;
    }
}

int get_min(vector<long long>&arr,vector<bool>visited){
    long long min_val = LLONG_MAX;
    int min_indx = -1;
    int size = arr.size();
    for(int i = 0;i<size;i++){
        long long a = arr[i];
        if(!visited[i] && min_val>a){
            min_val = a;
            min_indx = i;
        }
    }
    return min_indx;
}

void dijkstra(
    const vector<vector<edge>>&edges,vector<long long>& dist,
    const int& start
){
    int vert_count = edges.size();
    vector<bool>visited(vert_count,false);
    dist[start] = 0;
    
    for(int i = 1; i<vert_count;i++){
        int u = get_min(dist,visited);
        if (u == -1 || u == INF){break;}
        visited[u] = true;
        for(const edge& v:edges[u]){
            relax(u,v.to,dist,v);
        }
    }
}

int main(){
    int e,v;
    cin>>v>>e;
    vector<long long> dist(v+1,INF);
    vector<vector<edge>>edges(v+1);
    for(int i = 0;i<e;i++){
        int a,b,c;
        cin>>a>>b>>c;
        edges[a].push_back({b,c});
    }
    int start,end;
    cin>>start>>end;
    dijkstra(edges,dist,start);
    if (dist[end] != INF){
        cout<<dist[end];
    }
    else{
        cout<<-1;
    }
    
}
#include<vector>
#include<iostream>
#include<climits>
#include<queue>

using namespace std;
const long long INF = 1e17;

struct edge{
    edge(int t,long long c):to(t),cost(c){}
    int to;
    long long cost;
};



void update_infinite_arbitrage_paths(
    const vector<vector<edge>>&edges,
    vector<long long>& dist,queue<int>& q
){
    while(!q.empty()){
        int u = q.front();
        dist[u] = LLONG_MIN;
        q.pop();
        for(const edge& e:edges[u]){
            if(dist[e.to] != LLONG_MIN){
                dist[e.to] = LLONG_MIN;
                q.push(e.to);
            }
        }
    }
}

bool has_negative_cycle(const vector<vector<edge>>&edges,const int& start,vector<long long>& dist,vector<int>&prev){
    bool has = false;
    int vert_count = edges.size();
    queue<int> inf_arb_nodes;
    dist[start] = 0;
    for(int i = 1;i<=vert_count;i++){
        for(int v = 1;v<vert_count;v++){
            if(dist[v] == INF) continue;
            for(const edge&e:edges[v]){
                if(dist[e.to]>dist[v]+e.cost){
                    dist[e.to] = dist[v]+e.cost;
                    prev[e.to] = v;

                    if(i == vert_count){
                        has = true;
                        inf_arb_nodes.push(e.to);
                    }
                    
                }
            }
        }
    }
    update_infinite_arbitrage_paths(edges,dist,inf_arb_nodes);
    return has;
}


int main(){
    int e,v;
    cin>>v>>e;
    vector<long long> dist(v+1,INF);
    vector<int>prev(v+1,-1);
    vector<vector<edge>>edges(v+1);
    for(int i = 0;i<e;i++){
        int a,b,c;
        cin>>a>>b>>c;
        edges[a].push_back({b,c});
    }
    int start;
    cin>>start;
    has_negative_cycle(edges,start,dist,prev);
    for(int i = 1;i<=v;i++){
        if (dist[i]!= LLONG_MIN && dist[i] != INF){
            cout<<dist[i]<<"\n";
        }
        else if(dist[i] == LLONG_MIN ){
            cout<<"-\n";
        }
        else if(dist[i] == INF ){
            cout<<"*\n";
        }

    }
}
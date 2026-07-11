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


bool has_negative_cycle(const vector<vector<edge>>&edges,const int& start,vector<long long>& dist){
    int vert_count = edges.size();
    for(int i = 1;i<=vert_count;i++){
        for(int v = 1;v<vert_count;v++){
            for(const edge&e:edges[v]){
                if(dist[e.to]>dist[v]+e.cost){
                    if(i == vert_count){return true;}
                    dist[e.to] = dist[v]+e.cost;
                }
            }
        }
    }
    return false;
}


int main(){
    int e,v;
    cin>>v>>e;
    vector<long long> dist(v+1,0);
    vector<vector<edge>>edges(v+1);
    for(int i = 0;i<e;i++){
        int a,b,c;
        cin>>a>>b>>c;
        edges[a].push_back({b,c});
    }
    cout<<has_negative_cycle(edges,1,dist);
}
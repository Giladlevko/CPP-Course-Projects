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
typedef pair<long long,int> point;

void relax(const int& u,const int& v,vector<long long>& dist,const edge& e,
    priority_queue<point, vector<point>, greater<point>>&pq){
    if (dist[v]>dist[u]+e.cost){
        dist[v]=dist[u]+e.cost;
        pq.push({dist[v],v});
    }
}


vector<vector<edge>> reverse_graph(const vector<vector<edge>>& edges){
    int vert_count = edges.size();
    vector<vector<edge>> reversed(vert_count);
    for(int i = 1;i<vert_count;i++){
        for(const edge& j:edges[i]){
            reversed[j.to].push_back(edge(i,j.cost));
        }
    }
    return reversed;
}


long long shortest_dist(
    const vector<long long>&dist,
    const vector<long long>&r_dist
){
    long long distance = INF;
    int best = -1;
    int size = dist.size();
    for(int u = 1;u<size;u++){
        //cout<<"distances of "<<u<<": "<<dist[u]<<" "<<r_dist[u]<<"\n";
        if(dist[u] == INF || r_dist[u] == INF){continue;}
        if (dist[u]+r_dist[u]<distance){
            best = u;
            distance = dist[u]+r_dist[u];
        }
    }
    return distance;
}



long long bi_dijkstra(
    const vector<vector<edge>>&edges,const vector<vector<edge>>&r_edges,
    const int& start,const int& end
){
    if(start == end){return 0;}
    int vert_count = edges.size();
    vector<long long> dist(vert_count,INF);
    vector<long long> r_dist(vert_count,INF);
    
    vector<bool>visited(vert_count,false);
    vector<bool>r_visited(vert_count,false);

    priority_queue<point, vector<point>, greater<point>> minPQ;
    priority_queue<point, vector<point>, greater<point>> r_minPQ;

    dist[start] = 0;
    r_dist[end] = 0;
    minPQ.push({0,start});
    r_minPQ.push({0,end});
    
    while (!minPQ.empty() && !r_minPQ.empty()){
        int u = minPQ.top().second;
        minPQ.pop();
        if (u == -1 || u == INF){break;}
        visited[u] = true;

        for(const edge& v:edges[u]){
            relax(u,v.to,dist,v,minPQ);

        }
        if(r_visited[u]){return shortest_dist(dist,r_dist);}

        int r_u = r_minPQ.top().second;
        r_minPQ.pop();
        if (r_u == -1 || r_u == INF){break;}
        r_visited[r_u] = true;

        for(const edge& v:r_edges[r_u]){
            relax(r_u,v.to,r_dist,v,r_minPQ);

        }
        if(visited[r_u]){return shortest_dist(dist,r_dist);}
    }
    return INF;
}

int main(){
    int e,v;
    cin>>v>>e;

    vector<vector<edge>>edges(v+1);
    

    for(int i = 0;i<e;i++){
        int a,b,c;
        cin>>a>>b>>c;
        edges[a].push_back({b,c});
    }
    vector<vector<edge>>r_edges = reverse_graph(edges);
    int q;
    cin>>q;
    for(int i = 0;i<q;i++){
        int start,end;
        cin>>start>>end;
        long long result = bi_dijkstra(edges,r_edges,start,end);
        if (result != INF){
            cout<<result<<"\n";
        }
        else{
            cout<<-1<<"\n";
        }
    }
    
}
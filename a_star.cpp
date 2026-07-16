#include<vector>
#include<iostream>
#include<climits>
#include<queue>
#include<cmath>

using namespace std;
const long long INF = 1e17;

struct edge{
    edge(int t,long long c):to(t),cost(c){}
    int to;
    long long cost;
};
typedef pair<long long,int> point;


long double distance(
    const int& u,const int& v,
    const vector<long long>& x,const vector<long long>& y
){
    long long dx = x[u]-x[v];
    long long dy = y[u] - y[v];
    return(sqrt(dx*dx + dy*dy));
}

long long potential(
    const int& u,const int& start,const int& end,
    const vector<long long>& x,const vector<long long>& y
){
    return( (distance(u,end,x,y) - distance(start,u,x,y)) / 2.0 );
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


long long bi_a_star(
    const vector<vector<edge>>&edges,const vector<vector<edge>>&r_edges,
    const int& start,const int& end,const vector<long long>& x,const vector<long long>& y
){
    if(start == end){return 0;}
    long long best_distance = INF;
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

        if(minPQ.top().first + r_minPQ.top().first >= best_distance){
            break;
        }

        int u = minPQ.top().second;
        minPQ.pop();
        if (u == -1 || u == INF){break;}
        visited[u] = true;

        for(const edge& v:edges[u]){
            if (dist[v.to]>dist[u]+v.cost){
                dist[v.to]=dist[u]+v.cost;
                minPQ.push({dist[v.to]+potential(v.to,start,end,x,y),v.to});
                if(r_dist[v.to]!=INF){
                    best_distance = min(best_distance,r_dist[v.to]+dist[v.to]);
                }
            }

        }
        
        int r_u = r_minPQ.top().second;
        r_minPQ.pop();
        if (r_u == -1 || r_u == INF){break;}
        r_visited[r_u] = true;

        for(const edge& v:r_edges[r_u]){
            if (r_dist[v.to]>r_dist[r_u]+v.cost){
                r_dist[v.to] = r_dist[r_u]+v.cost;
                r_minPQ.push({r_dist[v.to]+potential(v.to,end,start,x,y),v.to});
                if(dist[v.to]!=INF){
                    best_distance = min(best_distance,r_dist[v.to]+dist[v.to]);
                }

            }
        }
    }
    return best_distance;
}

int main(){
    int e,v;
    cin>>v>>e;

    vector<vector<edge>>edges(v+1);
    vector<long long>x(v+1),y(v+1);

    for(int i = 1;i<v+1;i++){
        int px,py;
        cin>>px>>py;
        x[i] = px;
        y[i] = py;
    }

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
        long long result = bi_a_star(edges,r_edges,start,end,x,y);
        if (result != INF){
            cout<<result<<"\n";
        }
        else{
            cout<<-1<<"\n";
        }
    }
    
}
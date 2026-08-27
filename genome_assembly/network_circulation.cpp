#include<vector>
#include<queue>
#include<iostream>
#include<algorithm>
using namespace std;

void update_residual(
    vector<vector<long long>>& graph,
    const vector<int>& parent,
    const int& s,const int& t,
    const long long& flow
){
    int curr = t;
    while(curr != s){
        int child = curr;
        curr = parent[curr];
        graph[curr][child] -= flow;
        graph[child][curr] += flow;
    }
}



bool bfs_has_path(
    vector<vector<long long>>& graph,
    vector<int>& parents,
    const int& s, const int& t
){ 
    fill(parents.begin(),parents.end(),-1);
    parents[s] = s;
    queue<int> q;
    q.push(s);
    while(!q.empty()){
        int u = q.front();
        q.pop();
        for(int v = 1; v<graph.size();v++){
            if(parents[v] == -1 && graph[u][v]>0){
                q.push(v);
                parents[v] = u;
                if(v==t){return true;}
            }
        }
    }
    return (parents[t] != -1);
}

long long find_min_flow(
    const vector<vector<long long>>&graph,
    const vector<int>&parent,
    const int& s,const int& t
){
    int curr = t;
    long long min_flow = 1e18;
    while(curr != s){
        int child = curr;
        curr = parent[curr];
        long long capacity = graph[curr][child];
        min_flow = min(min_flow,capacity);
    }
    return min_flow;
}

long long get_max_flow(
    vector<vector<long long>>& graph,
    const int& s, const int& t
){
    vector<int>parent(graph.size(),-1);
    long long flow = 0;
    while(bfs_has_path(graph,parent,s,t)){
        //gets the smallest flow in the path from s to t
        // that way we know what the bottle-neck is
        long long min_flow = find_min_flow(graph,parent,s,t);
        update_residual(graph,parent,s,t,min_flow);
        flow +=min_flow;
    }
    return flow;
}

//adding the source and target by connecting the s->v with the in sum for that
//vertex and v->t with the out sum for that vertex
//this will make the max_flow try to reach the sum of min_flow (l)
//if it does we satisfied the requirements and we have circulation
void fill_s_and_t_edges(
    vector<vector<long long>>& graph,
    vector<long long>&in,
    vector<long long>&out
){
    int size = graph.size();
    int s = 0;
    int t = size -1;
    for(int i = 1; i<t; i++){
        graph[s][i] = in[i];
        graph[i][t] = out[i];
    }
}

struct edge{
    edge(int u,int v,int l,int c):u(u),v(v),l(l),c(c){}
    int u,v,l,c;
};


void has_circulation(
    const long long&max_flow,
    const long long&low_sum,
    const vector<edge>&edges,
    vector<vector<long long>>&initial_graph,
    const vector<vector<long long>>&residual_graph
){
    if(max_flow<low_sum){
        cout<<"NO";return;
    }
    cout<<"YES\n";
    for(const edge& e:edges){
        int cap = e.c-e.l;
        long long max_flow_u_to_v = initial_graph[e.u][e.v] - residual_graph[e.u][e.v];
        //pushing flow from u->v increases flow of v->u, making the residual > initial
        //if the difference is <0 then the flow pushed from u->v is 0 since v->u was the one used
        max_flow_u_to_v = max_flow_u_to_v>0 ? max_flow_u_to_v : 0;
        //need to have this check because of parallel edges that add up in the graph
        long long actual_passed = max_flow_u_to_v<cap ? max_flow_u_to_v : cap;
        //decrement so parallel edges only take remainig flow 
        initial_graph[e.u][e.v]-=actual_passed;
        cout<< actual_passed + e.l << "\n";
    }
}


int main(){
    int vert_count,edge_count;
    cin>>vert_count>>edge_count;
    vector<vector<long long>>graph(vert_count+2,vector<long long>(vert_count+2,0));
    vector<long long>in(vert_count+1,0),out(vert_count+1,0);
    vector<edge>edges;
    int s = 0;
    int t = vert_count+1;
    long long low_sum = 0;
    for(int i = 0; i<edge_count; i++){
        int u,v,l,c;
        cin>>u>>v>>l>>c;
        graph[u][v] += c-l;
        //need to track of the in and out to fill the edges for s->v (in)
        //and v->t. we use that to know if all the min_flow (l) were satisfied
        //by checking if the max_flow == the sum of all the min_flow (l)
        out[u]+=l;
        in[v]+=l;
        //need to keep track of the min_flow (l) sum to check if it equal to max_flow later
        low_sum += l;
        //need to keep trak of the edges for the output later
        edges.push_back({u,v,l,c});
    }
    fill_s_and_t_edges(graph,in,out);

    vector<vector<long long>>initial_graph = graph;

    long long max_flow = get_max_flow(graph,s,t);

    has_circulation(max_flow,low_sum,edges,initial_graph,graph);
}
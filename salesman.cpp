#include<vector>
#include<iostream>
#include<climits>
#include<queue>
#include<cmath>
#include<algorithm>
//for the Traveling salseman dynamic programming algorithm
//I used this video which I found very helpful: 
//Traveling Salesman Problem | Dynamic Programming | Graph Theory | by WilliamFiset
//https://youtu.be/cY4HiiFHO1o?si=QahRhwghxBa_yPuv
using namespace std;
const long long INF = 1e17;

struct edge{
    edge(int t,long long c):to(t),cost(c){}
    int to;
    long long cost;
};

struct shortcut {
    int from;
    int to;
    long long cost;
};

typedef pair<long long,int> point;


void witness_search(const int& start,const int& avoid,
    const long long& max_dist,const vector<vector<edge>>&edges,
    const vector<bool>&contracted,vector<long long>& dist,vector<int>&touched_dist,vector<int>&depth)
{
    for(const int &i:touched_dist){
        dist[i] = INF;
        depth[i] = 0;
    }
    touched_dist.clear();

    int vert_count = edges.size();
    
    priority_queue<point, vector<point>, greater<point>> minPQ;
    dist[start] = 0;
    depth[start] = 0;

    touched_dist.push_back(start);

    minPQ.push({0,start});
    while(!minPQ.empty()){
        long long u_distance = minPQ.top().first;
        int u = minPQ.top().second;
        minPQ.pop();

        if(u_distance > max_dist){break;}
        if(u_distance>dist[u]){continue;}
        
        for(const edge& e:edges[u]){
            if(e.to == avoid || contracted[e.to]){continue;}
            if(dist[e.to] > u_distance +e.cost){
                if(dist[e.to] == INF){touched_dist.push_back(e.to);}

                int new_depth = depth[u]+1;
                depth[e.to] = new_depth;
                if(new_depth>5){continue;}

                dist[e.to] = u_distance + e.cost;
                minPQ.push({dist[e.to],e.to});
            }
        }
    }
}

int calc_importance(
    const int& u, const vector<vector<edge>>& edges, const vector<vector<edge>>& r_edges,
    const vector<bool>&contracted, const vector<int>&level,vector<shortcut>&shortcuts,
    vector<long long>&witness_dist, vector<int>&touched_nodes,vector<int>&depth
){
    int ed = 0;//edge difference
    int cn = 0;//contracted neighbors
    int estimated_level = level[u];
    int neighbor_max_level = 0;

    shortcuts.clear();
    int shortcuts_needed = 0;//also shortcut cover
    int in_deg = 0;
    int out_deg = 0;

    for(const edge& e:edges[u]){
        if(!contracted[e.to]){out_deg++;}
        else{cn++;}
    }
    for(const edge& e:r_edges[u]){if(!contracted[e.to]){in_deg++;}}

    
    for(const edge& in_e:r_edges[u]){

        if(contracted[in_e.to]){continue;}
        long long max_u_path = -1;

        for(const edge& out_e:edges[u]){

            if (contracted[out_e.to] || in_e.to == out_e.to){continue;}

            neighbor_max_level = max({neighbor_max_level,level[out_e.to],level[in_e.to]});
            
            long long path_with_u = out_e.cost + in_e.cost;
            //keeps the longest distance so i know how long to search for a witness
            max_u_path = max(max_u_path,path_with_u);
        }

        if(max_u_path == -1){continue;}
//from every out edge i look at everywhere i can go until i hit the max distance. 
//once that happens i looked at everywhere that a witness path could be
// between u and and some in coming node w1 and an ougoing node w2
       witness_search(
            in_e.to,u,max_u_path,edges,contracted,witness_dist,touched_nodes,depth
        );

        for(const edge& out_e:edges[u]){

            if (contracted[out_e.to] || in_e.to == out_e.to){continue;}
            //if i find a witness path the distance is saved in witness_dist otherwise it'll be INF
            //if that distance is better than a path I have through u than Nothing happens
            //But if such path doesnt exist (INF) or its still worse, I need to make a shortcut
            long long path_with_u = in_e.cost+out_e.cost;
            if(witness_dist[out_e.to]>path_with_u){
                shortcuts_needed++;
                shortcuts.push_back({in_e.to,out_e.to,path_with_u});
            }
        }
    }
    ed = shortcuts_needed -in_deg -out_deg;
    estimated_level = max(estimated_level,neighbor_max_level + 1);


    return ed + cn + shortcuts_needed + estimated_level;
}

typedef pair<int,int> node_imp;//importance,node

//adds all the shorcuts I have accumulated 
//but looks if such shortcut from the two nodes already exist.
//if so, I make it have the smallest distance Ibetween the duplicate shortcuts.
void add_shortcut(vector<vector<edge>>& graph, int from, int to, long long cost) {
    for (edge &e : graph[from]) {
        if (e.to == to) {
            if (cost < e.cost)
                e.cost = cost;
            return;
        }
    }
    graph[from].push_back({to, cost});
}

void preprocess_graph(vector<vector<edge>>&edges,vector<vector<edge>>&r_edges,vector<int>&level){
    priority_queue<node_imp,vector<node_imp>,greater<node_imp>> imp_Q;
    int v_count = edges.size();
    
    vector<bool> contracted(v_count, false);
    int current_level = 0;

    vector<shortcut>shortcuts;
    vector<long long>witness_dist(edges.size(),INF);
    vector<int>depth(edges.size(),0);
    vector<int>touched_nodes;

    //initial
    for(int i = 1; i < v_count; i++){
        int i_imp = calc_importance(i,edges,r_edges,contracted,level,shortcuts,witness_dist,touched_nodes,depth);
        imp_Q.push({i_imp,i});
    }
    while(!imp_Q.empty()){
        int u = imp_Q.top().second;
        imp_Q.pop();
        if(contracted[u]){continue;}

        //re-check
        int u_imp = calc_importance(u,edges,r_edges,contracted,level,shortcuts,witness_dist,touched_nodes,depth);
        if(!imp_Q.empty() && u_imp > imp_Q.top().first){
            imp_Q.push({u_imp,u});
            continue;
        }
        contracted[u] = true;
        level[u] = current_level++;
        for(const shortcut& s:shortcuts){
            
            add_shortcut(edges,s.from,s.to,s.cost);
            add_shortcut(r_edges,s.to,s.from,s.cost);
        }
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


long long bi_dijkstra(
    const vector<vector<edge>>&edges,const vector<vector<edge>>&r_edges,
    const int& start,const int& end,
    const vector<int>&level,vector<long long>& dist,vector<long long>& r_dist,
    vector<int>&touched,vector<int>&r_touched
){
    if(start == end){return 0;}

    for(const int&i:touched){dist[i] = INF;}
    for(const int&i:r_touched){r_dist[i] = INF;}
    touched.clear();r_touched.clear();


    long long best_distance = INF;
    int vert_count = edges.size();

    priority_queue<point, vector<point>, greater<point>> minPQ;
    priority_queue<point, vector<point>, greater<point>> r_minPQ;

    dist[start] = 0;
    r_dist[end] = 0;
    minPQ.push({0,start});
    r_minPQ.push({0,end});

    touched.push_back(start);
    r_touched.push_back(end);

    
    while (!minPQ.empty() || !r_minPQ.empty()){

        long long forward_best = minPQ.empty() ? INF : minPQ.top().first;
        long long reverse_best = r_minPQ.empty() ? INF : r_minPQ.top().first;

        //if both of my best options are worst than my best_distance I can stop
        if(forward_best >= best_distance && reverse_best >= best_distance){
            break;
        }

        if(!minPQ.empty()){
            long long d = minPQ.top().first;
            int u = minPQ.top().second;
            minPQ.pop();
            if(d <= dist[u] && d < best_distance){

                for(const edge& v:edges[u]){
                    if (level[u]>level[v.to]){continue;}
                    if (dist[v.to]>dist[u]+v.cost){
                        dist[v.to]=dist[u]+v.cost;
                        touched.push_back(v.to);
                        minPQ.push({dist[v.to],v.to});
                        if(r_dist[v.to]!=INF){
                            best_distance = min(best_distance,r_dist[v.to]+dist[v.to]);
                        }
                    }
                }
            }
        }
        if(!r_minPQ.empty()){
            long long d = r_minPQ.top().first;
            int r_u = r_minPQ.top().second;
            r_minPQ.pop();
            if(d <= r_dist[r_u] && d < best_distance){

                for(const edge& v:r_edges[r_u]){
                    if (level[r_u]>level[v.to]){continue;}
                    if (r_dist[v.to]>r_dist[r_u]+v.cost){
                        r_dist[v.to] = r_dist[r_u]+v.cost;
                        r_touched.push_back(v.to);
                        r_minPQ.push({r_dist[v.to],v.to});
                        if(dist[v.to]!=INF){
                            best_distance = min(best_distance,r_dist[v.to]+dist[v.to]);
                        }

                    }
                }
            }
        }
    }
    return best_distance;
}

void build_tsp_matrix(
    vector<vector<long long>>& tsp_m, const vector<int>&node_map,
    const vector<vector<edge>>& edges, const vector<vector<edge>>& r_edges, 
    const vector<int>& level,
    vector<long long>& dist, vector<long long>& r_dist,
    vector<int>& touched, vector<int>& r_touched
){
    int k = tsp_m.size();
    for(int i = 0; i<k;i++){
        int s = node_map[i];
        for(int j = 0;j<k;j++){
            if(i==j){
                tsp_m[i][j] = 0;
                continue;
            }
            int t = node_map[j];

            long long result = bi_dijkstra(
                edges,r_edges,s,t,level,dist,r_dist,touched,r_touched
            );

            tsp_m[i][j] = result;
        }
    }
}

void setup(
    vector<vector<long long>>&memo,const vector<vector<long long>>& tsp_m,
    const int& start,const int& k
){
    for(int i = 0;i<k;i++){
        if(i==start){continue;}
        memo[i][1 << start | 1 << i] = tsp_m[start][i];
    }
}

//returns true if the i'th bit in the subset is not there
bool not_in(int i,int subset){
    return((1 << i & subset) == 0);
}

void gen_combinations(int set,int at,int r,int k,vector<int>& sets){
    if(r == 0){
        sets.push_back(set);
    }
    else{
        for(int i = at;i<k;i++){
            //flip on i'th bit
            set = set | 1 << i;

            gen_combinations(set,i+1,r-1,k,sets);

            //backtrack and flip off i'th bit
            set = set & ~(1 << i);
        }
    }
}

//generates all bit sets of size k with r bits set to 1
//combinations(3,4) = [0111, 1011, 1101, 1110]
vector<int> combinations(int r,int k){
    vector<int> sets;
    gen_combinations(0,0,r,k,sets);
    return sets;

}

void solve(
    vector<vector<long long>>&memo,const vector<vector<long long>>& tsp_m,
    const int& start,const int& k
){
    for(int r = 3;r<=k;r++){
        vector<int> subsets = combinations(r,k);
        for(int& subset:subsets){
            if(not_in(start,subset)){continue;}
            for(int next = 0; next<k; next++){
                if(next == start || not_in(next,subset)){continue;}
                //the subset state without the next node
                //essentially turns off the next'th bit in the subset
                int state = subset ^ (1 << next);
                long long min_dist = INF;

                for(int end_node = 0; end_node < k; end_node++){
                    if(end_node == start || end_node == next || not_in(end_node,subset)){
                        continue;
                    }
                    //checks the dist up until I arrived to the next node
                    //then adds the distance from the last node i was in to the next node
                    //giving me a correct distance for the current path
                    long long new_dist = memo[end_node][state] + tsp_m[end_node][next];
                    if (new_dist < min_dist){min_dist = new_dist;}
                }
                memo[next][subset] = min_dist;
            }
        }
    }

}

long long get_min_cost(
    vector<vector<long long>>&memo,const vector<vector<long long>>& tsp_m,
    const int& start,const int& k
){
    //the END_STATE is the bit mask with k bits set to one -> 2^k - 1
    const int END_STATE = (1 << k) -1;
    long long min_tour_cost = INF;
    for(int end_node = 0; end_node<k; end_node++){
        if(end_node == start){continue;}
        //looks at all the possible end nodes for when all the nodes have been visited
        //and sees which end node gives the min distance for the route + the journey from the end to the start
        long long tour_cost = memo[end_node][END_STATE] + tsp_m[end_node][start];
        if(tour_cost<min_tour_cost){min_tour_cost = tour_cost;}
    }
    return min_tour_cost;
}

long long tsp_dp(const vector<vector<long long>>& tsp_m,const int& start){
    int k = tsp_m.size();
    vector<vector<long long>>memo(k,vector<long long>((1 << k),-1));//2^k column size
    setup(memo,tsp_m,start,k);
    solve(memo,tsp_m,start,k);
    long long min_cost = get_min_cost(memo,tsp_m,start,k);
    return min_cost;
}


int main(){
    int e,v;
    cin>>v>>e;

    vector<vector<edge>>edges(v+1);
    vector<long long> dist(v+1,INF);
    vector<long long> r_dist(v+1,INF);
    vector<int>touched,r_touched;
    vector<int> level(v+1, 0);

    

    for(int i = 0;i<e;i++){
        int a,b,c;
        cin>>a>>b>>c;
        edges[a].push_back({b,c});
    }
    vector<vector<edge>>r_edges = reverse_graph(edges);
    preprocess_graph(edges,r_edges,level);
    cout<<"Ready\n";
    int q;
    cin>>q;
    for(int i = 0;i<q;i++){
        int k,start = 0;
        cin>>k;
        vector<int>node_map(k);
        vector<vector<long long>> tsp_m(k,vector<long long>(k));
        for(int j = 0;j<k;j++){
            int node;
            cin>>node;
            node_map[j] = node;
        }
        build_tsp_matrix(tsp_m,node_map,edges,r_edges,level,dist,r_dist,touched,r_touched);
        long long result = tsp_dp(tsp_m,start);
        if(result == INF){cout<<"-1\n";}
        else{
            cout<<result<<"\n";
        }
        
    }
    
}
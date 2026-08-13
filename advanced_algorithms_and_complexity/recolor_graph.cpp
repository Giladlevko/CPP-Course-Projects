#include<vector>
#include<iostream>
#include<string>

using namespace std;

//there are 3 exam dates
//students are assigned exam dates wrongly
//we know they can come at any of the other two dates
//we know they can't have the same date as their close friends


struct edge{
    edge(int f, int t):from(f),to(t){}
    edge() = default;
    int from;
    int to;
};

const int COLOR_COUNT = 3;

//maps to a flat index from vertex and color
int sat_index(int vertex,int color){
    return (vertex * COLOR_COUNT + color + 1);
}

vector<int> char_to_color(const string& s){
    vector<int>result;
    int col;
    for(const char&c:s){
        switch(c){
            case 'R':col = 0;break;
            case 'B':col = 1;break;
            case 'G':col = 2;break;
        }
        result.push_back(col);
    }
    return result;
}

char color_to_char(const int& i){
    switch(i){
            case 0:return 'R';
            case 1:return 'B';
            case 2:return 'G';
            default: return 'R';
    }
}

string sat_to_colors(const vector<int>& assignment,const int&vert_count){
    string result = "";
    for(int v = 0; v<vert_count; v++){
        for(int c = 0; c<COLOR_COUNT; c++){
            int var_indx = sat_index(v,c) - 1;
            if(assignment[var_indx] == 1){
                result+=color_to_char(c);
                break;
            }
        }
    }
    return result;
}


vector<vector<int>> graph_coloring_to_sat(
    const vector<edge>&graph,
    vector<int>&assigned_colors,
    const int& vert_count
){
    int size = graph.size();
    vector<vector<int>> clauses;

    //each vertex has at least one color of the colors he isn't assigned
    for(int v = 0; v<vert_count; v++){
        vector<int>clause;
        for(int c = 0; c<COLOR_COUNT; c++){
            if(assigned_colors[v] == c){continue;}
            int id = sat_index(v,c);
            clause.push_back(id);
        }
        clauses.push_back(clause);
    }

    //each vertex has at most one color of the colors he isn't assigned
    for(int v =0; v<vert_count; v++){
        
        for(int c1 = 0; c1<COLOR_COUNT; c1++){
            if(assigned_colors[v] == c1){continue;}
            int id_c1 = -1*sat_index(v,c1);
            for(int c2 = c1+1; c2<COLOR_COUNT; c2++){
                if(assigned_colors[v] == c2){continue;}
                int id_c2 = -1*sat_index(v,c2);

                vector<int>clause;

                clause.push_back(id_c1);
                clause.push_back(id_c2);

                clauses.push_back(clause);
            }
        } 
    }

    //vertex cannot have same color as edge
    for(int e = 0; e<size; e++){
        for(int c = 0; c<COLOR_COUNT; c++){
            vector<int>clause;

            int id_from = -1 * sat_index(graph[e].from,c);
            int id_to = -1 * sat_index(graph[e].to,c);

            clause.push_back(id_from); clause.push_back(id_to);

            clauses.push_back(clause);
        }
    }
    return clauses;
}




int get_node_index(const int& i,const int& var_count){
    return i<0? -i-1+var_count : i-1;
}
int get_neg(const int& i,const int& var_count){
    if(i<var_count){return (i+var_count);}
    return (i-var_count);
}

vector<vector<int>> construct_implication(vector<vector<int>>clauses,int var_count){
    vector<vector<int>> implication(2*var_count);
    for(vector<int>&clause:clauses){
        int u = clause[0];
        int v = clause.size()>1? clause[1] : clause[0];
        int node_u = get_node_index(u,var_count);
        int node_v = get_node_index(v,var_count);
        int neg_u = get_neg(node_u,var_count);
        int neg_v = get_neg(node_v,var_count);
        //~u -> v
        implication[neg_u].push_back(node_v);
        //~v -> u
        implication[neg_v].push_back(node_u);

    }
    return implication;
}
/*******************************************************SCC ALGORITHM***************************************************/
void explore(
    const int& v_index,vector<bool>&visited,
    const vector<vector<int>>&edges,vector<int>&post_order
){
    visited[v_index] = true;
    for(const int& i:edges[v_index]){
        if (!visited[i]){
            explore(i,visited,edges,post_order);
        }
    }
    post_order.push_back(v_index);
}

void explore_and_assign_id(
    const int& v_index,vector<bool>&visited,
    const vector<vector<int>>&edges,
    const int&id,vector<int>&scc_id
){
    visited[v_index] = true;
    scc_id[v_index] = id;
    for(const int& i:edges[v_index]){
        if (!visited[i]){
            explore_and_assign_id(i,visited,edges,id,scc_id);
        }
    }
}


void dfs(const vector<vector<int>>&edges,vector<int>&post_order){
    int vert_count = edges.size();
    vector<bool>visited(vert_count,false);
    for(int v = 0;v<vert_count;v++){
        if(!visited[v]){
            explore(v,visited,edges,post_order);
        }
    }
}

vector<vector<int>> reverse_graph(const vector<vector<int>>&edges){
    int vert_count = edges.size();
    vector<vector<int>> reversed(vert_count);
    for(int i = 0;i<vert_count;i++){
        for(const int& j:edges[i]){
            reversed[j].push_back(i);
        }
    }
    return reversed;
}

vector<int> get_scc(const vector<vector<int>>&edges){
    int vert_count = edges.size();

    vector<int>scc_ids(vert_count);

    vector<vector<int>> reversed_edges = reverse_graph(edges);
    vector<int> reversed_post_order;
    dfs(reversed_edges,reversed_post_order);

    int scc_id = 0;

    vector<bool>visited(vert_count,false);
    vector<int> post_order;
    vector<bool>in_scc(vert_count,false);
    vector<int>reversed_order;
    for(int v = reversed_post_order.size()-1;v>=0;v--){
        int vertex = reversed_post_order[v];
        if(!visited[vertex]){
            scc_id++;
            explore_and_assign_id(vertex,visited,edges,scc_id,scc_ids);
            

        }
    }
    return scc_ids;
}
/**************************************************************************************************************/


bool solve_2sat(vector<vector<int>>&clauses,const int&var_count,vector<int>&assignment){
    vector<vector<int>>implication = construct_implication(clauses,var_count);
    vector<int>scc_ids = get_scc(implication);
    for(int x_i = 0; x_i<var_count; x_i++){
        int neg = get_neg(x_i,var_count);
        if(scc_ids[x_i] == scc_ids[neg]){return false;}
        //if the id is smaller then its neg id then
        //it means the graph has ~x_i->x_i 
        //because smaller id means being deeper in the graph
        //and because of how implications work
        //setting the deeper is always safe because 
        //it cant break a promise
        assignment[x_i] = scc_ids[x_i]<scc_ids[neg] ? 1 : -1;
    }
    return true;
}

void handle_sat(vector<vector<int>>&clauses,const int&var_count,const int&vert_count){
    vector<int>result(var_count,0);
    bool sat = solve_2sat(clauses,var_count,result);
    if(sat){
       cout<<sat_to_colors(result,vert_count);
    }
    else{
        cout<<"Impossible";
    }
}




int main(){
    int vert_count,edge_count;
    cin>>vert_count>>edge_count;
    string colors;
    cin>>colors;
    vector<int>assigned_colors = char_to_color(colors);
    vector<edge>graph;
    for(int i = 0; i<edge_count; i++){
        int v,u;
        cin>>v>>u;
        v--,u--;
        graph.push_back(edge(v,u));
    }
    vector<vector<int>> clauses = graph_coloring_to_sat(graph,assigned_colors,vert_count);
    int var_count = vert_count * COLOR_COUNT;
    handle_sat(clauses,var_count,vert_count);


    return 0;
}
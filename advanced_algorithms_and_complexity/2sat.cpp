#include<vector>
#include<iostream>
#include<algorithm>

using namespace std;

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

void handle_sat(vector<vector<int>>&clauses,const int&var_count){
    vector<int>result(var_count,0);
    bool sat = solve_2sat(clauses,var_count,result);
    if(sat){
        cout<<"SATISFIABLE\n";
        for(int i = 0; i<var_count; i++){
            cout<<(i+1)*result[i]<<" ";
        }
    }
    else{
        cout<<"UNSATISFIABLE";
    }
}


int main(){
    int vars,clause_count;
    cin>>vars>>clause_count;
    vector<vector<int>>clauses(clause_count);
    for(int i = 0; i<clause_count; i++){
        int x_i,x_j;
        cin>>x_i>>x_j;
        clauses[i] = {x_i,x_j};
    }
    handle_sat(clauses,vars);
}
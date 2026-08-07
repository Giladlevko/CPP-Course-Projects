#include<vector>
#include<iostream>

using namespace std;

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


vector<vector<int>> graph_coloring_to_sat(const vector<edge>&graph,const int& vert_count){
    int size = graph.size();
    vector<vector<int>> clauses;

    //each vertex has at least one color
    for(int v = 0; v<vert_count; v++){
        vector<int>clause;
        for(int c = 0; c<COLOR_COUNT; c++){
            int id = sat_index(v,c);
            clause.push_back(id);
        }
        clauses.push_back(clause);
    }

    //each vertex has at most one color
    for(int v =0; v<vert_count; v++){
        
        for(int c1 = 0; c1<COLOR_COUNT; c1++){
            int id_c1 = -1*sat_index(v,c1);
            for(int c2 = c1+1; c2<COLOR_COUNT; c2++){
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


int main(){
    int vert_count,edge_count;
    cin>>vert_count>>edge_count;
    vector<edge>graph(edge_count);
    for(int i = 0; i<edge_count; i++){
        int u,v;
        cin>>u>>v;
        graph[i] = edge(u-1,v-1);
    }
    //for each vertex there are 3 vars, one for each color
    int var_count = 3 * vert_count;
    vector<vector<int>>clauses = graph_coloring_to_sat(graph,vert_count);
    cout<<clauses.size()<<" "<<var_count<<" \n";
    for(int i = 0; i<clauses.size(); i++){
        for(int j = 0; j<clauses[i].size(); j++){
            cout<<clauses[i][j]<<" ";
        }
        cout<<"0\n";
    }
}
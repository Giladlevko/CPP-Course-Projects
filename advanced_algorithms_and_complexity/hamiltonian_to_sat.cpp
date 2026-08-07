#include<vector>
#include<iostream>

using namespace std;


//maps to a flat index from vertex and path position
int sat_index(int vertex,int path_pos,int vert_count){
    return (vertex * vert_count + path_pos + 1);
}


vector<vector<int>> hamiltonian_to_sat(const vector<vector<bool>>&graph,const int&vert_count){
    int size = graph.size();
    vector<vector<int>> clauses;

    //each vertex belongs to some position in path
    for(int v = 0; v<vert_count; v++){
        vector<int>clause;
        for(int p = 0; p<vert_count; p++){
            int id = sat_index(v,p,vert_count);
            clause.push_back(id);
        }
        clauses.push_back(clause);
    }

    //each vertex appears once in path
    for(int v = 0; v<vert_count; v++){
        for(int p1 = 0; p1<vert_count; p1++){
            int id_p1 = -1 * sat_index(v,p1,vert_count);
            for(int p2 = p1+1; p2<vert_count; p2++){
                vector<int>clause;
                int id_p2 = -1 * sat_index(v,p2,vert_count);
                //[-i1,-i2] => they can't be the same must be opposite
                clause.push_back(id_p1);clause.push_back(id_p2);
                clauses.push_back(clause);
            }
        }
    }

    //each position in a path belongs some vertex
    for(int p = 0; p<vert_count; p++){
        vector<int>clause;
        for(int v = 0; v<vert_count; v++){
            int id = sat_index(v,p,vert_count);
            clause.push_back(id);
        }
        clauses.push_back(clause);
    }

    //no two vertices occupy the same positon
    for(int p = 0; p<vert_count; p++){
        for(int v1 = 0; v1<vert_count; v1++){
            int id_v1 = -1 * sat_index(v1,p,vert_count);
            for(int v2 = v1+1; v2<vert_count; v2++){
                vector<int>clause;
                int id_v2 = -1 * sat_index(v2,p,vert_count);
                clause.push_back(id_v1);clause.push_back(id_v2);
                clauses.push_back(clause);
            }
        }
    }

    //two succsesive verices must be connected by an edge
    //i.e if there is no edge then they cant be succsesive
    for(int v1 = 0; v1<vert_count; v1++){
        for(int v2 = 0; v2<vert_count; v2++){
            if(v1 != v2 && !graph[v1][v2]){
                for(int p = 0; p<vert_count-1; p++){
                    vector<int>clause;
                    int id_v1 = -1*sat_index(v1,p,vert_count);
                    int id_v2 = -1*sat_index(v2,p+1,vert_count);
                    clause.push_back(id_v1);clause.push_back(id_v2);
                    clauses.push_back(clause);
                }
            }
        }
    }
    return clauses;
}


int main(){
    int vert_count,edge_count;
    cin>>vert_count>>edge_count;
    vector<vector<bool>> graph(vert_count,vector<bool>(vert_count,false));
    for(int e = 0; e<edge_count; e++){
        int v1,v2;
        cin>>v1>>v2;
        graph[v1-1][v2-1] = true; graph[v2-1][v1-1] = true;
    }

    vector<vector<int>>clauses = hamiltonian_to_sat(graph,vert_count);
    int var_count = vert_count * vert_count;
    int clause_count = clauses.size();
    cout<<clause_count<<" "<<var_count<<"\n";
    
    for(int i = 0; i<clause_count; i++){
        for(int j = 0; j<clauses[i].size(); j++){
            cout<<clauses[i][j]<<" ";
        }
        cout<<"0\n";
    }
}
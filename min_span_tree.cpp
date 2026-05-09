//Name: Mr.lad
//Date:5.8.2026
//
//Graph Algorithms Project
//
//This program implements an undirected weighted graph using
// an adjacency list representation. The graph can either be
// randomly generated or read from a file.
//
//The program includes:
//
// 1. Dijkstra's Shortest Path Algorithm
//    - Finds the shortest path from a source node to all
//      other nodes using greedy relaxation.
//
// 2. Prim's Minimum Spanning Tree Algorithm
//    - Builds a minimum spanning tree by repeatedly selecting
//      the lowest-cost edge connecting a visited node to an
//      unvisited node.
//
// Each edge stores:
//    - destination node
//    - edge weight (cost)
//    - edge color
//
//The program can print shortest paths, MST edges,
// edge colors, and total MST cost.


#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <climits>
using namespace std;

enum class edge_color{GREEN,BLUE,RED,NONE};

ostream& operator<<(ostream& out, const edge_color& c){
    switch (c){
        case edge_color::GREEN: out<<"GREEN";break;
        case edge_color::BLUE: out<<"BLUE";break;
        case edge_color::RED: out << "RED";break;
        case edge_color::NONE: out<< "NONE";break;
        default: out << "UNKNOWN"; break;
    }
    return out;
}

struct edge{
    int index;
    double cost;
    edge_color color;
};

class Graph{
    public:
        //Name: Graph
        //Input: none
        //Output: none (constructor)
        //Description: 
        // Initializes graph, generates random edges, and sets up data arrays.
        Graph(){
            construct_data();
            gen_graph();
        }
        //Name: Graph
        //Input: none
        //Output: none (constructor)
        //Description: 
        // Initializes graph by reading graph data from file.
        Graph(string file_name){
            read_graph(file_name);
            construct_data();

        }
        //Name: get_v_count
        //Input: none
        //Output: int
        //Description: 
        // Returns number of vertices in graph.   
        int get_v_count(){
            return V_count;
        }
        //Name: get_edge_value
        //Input: x,y: indexes of two nodes
        //Output: float
        //Description: 
        // Returns value of edge, if edge does not exist returns infinity.
        float get_edge_value(int x, int y){
            for( edge e: edge_list[x]){
                if (e.index == y){
                    return e.cost;
                }
            }
            return INF;
        }


        //Name: get_edge_color
        //Input: x,y: indexes of two nodes
        //Output: edge_color
        //Description: 
        // Returns color of edge, if edge does not exist returns NONE.
        edge_color get_edge_color(int x,int y){
            for( edge e: edge_list[x]){
                if (e.index == y){
                    return e.color;
                }
            }
            return edge_color::NONE;

        }
        
        //Name: get_e_count
        //Input: none
        //Output: int
        //Description: 
        // Returns number of edges in graph.
        int get_e_count(){
            return E_count;
        }

        //Name: use_dijkstra
        //Input: none
        //Output: void
        //Description: 
        // Runs Dijkstra shortest path algorithm.
        void use_dijkstra(){
            dikstra_shortest_path();
        }

        //Name: use_prim
        //Input: none
        //Output: void
        //Description: 
        // Runs Prim's MST algorithm.
        void use_prim(){
            prim_mst();
        }

        //Name: print_shortest_path
        //Input: int (target node)
        //Output: void
        //Description: 
        // Recursively prints shortest path from source to target.        
        void print_shortest_path(int parent_indx){
            if (parent_indx != -1){
                print_shortest_path(parent[parent_indx]);
                cout <<" :" << parent_indx;
            }
        }

        //Name: print_mst
        //Input: none
        //Output: void
        //Description: 
        // Prints MST in the form of edges, cost, color and finally total MST cost
        void print_mst(){
            float sum = 0;
            for (int i = 1; i<V_count;i++){
                int parent_indx = parent[i];
                if (parent_indx == -1) {
                    cout << "no parent for " << i << endl;
                    return;
                }
                cout << parent_indx << " - " << i;
                float edge_value = get_edge_value(parent_indx,i);
                edge_color color = get_edge_color(parent_indx,i);
                cout << " cost: "<<edge_value<<" Color: "<< color <<endl;
                sum+=edge_value;
            }
            cout <<"MST total cost: "<<sum <<endl;
        }
    
    private:
        int V_count = 15;
        int E_count = 0;
        static constexpr double INF = 1e10;
        double density = 0.2;
        vector<vector<edge>> edge_list;
        vector<double> dist;
        vector<bool> visited;
        vector<int> parent;
        vector<vector<int>> mst;

        //Name: construct_data
        //Input: none
        //Output: void
        //Description: 
        // Initializes visited, distance, parent, and MST vectors.
        void construct_data(){
            dist.assign(V_count, INF);
            visited.assign(V_count, false);
            parent.assign(V_count,-1);
            mst.assign(V_count,{});
            dist[0] = 0;
        }
        //Name: prob
        //Input: none
        //Output: double
        //Description: 
        // Generates random probability in range [0,1].
        double prob(){
            return (static_cast<double>(rand()) / RAND_MAX);
        }

        //Name: gen_graph
        //Input: none
        //Output: void
        //Description: 
        // Generates random undirected weighted graph using density parameter.
        void gen_graph(){
            edge_list.assign(V_count,{});
            for(int i = 0; i < V_count; ++i){
                for (int j = i; j < V_count; ++j){
                    if (prob()<density){
                        double edge_cost = prob()*9 +1;
                        edge_color color = static_cast<edge_color>(prob()*3);
                        edge_list[i].push_back({j,edge_cost,color});
                        edge_list[j].push_back({i,edge_cost,color});
                        E_count++;
                    }
                }
            }
        }

        //Name: read_graph
        //Input: string of file name
        //Output: void
        //Description: 
        // Reads undirected weighted graph from inputed file name.
        void read_graph(string file_name){
            int from;
            int to;
            double cost;
            int color;
            ifstream file(file_name);
            file >> V_count;
            edge_list.assign(V_count,{});
            
            while (file >> from >> to >> cost >> color){
                edge_list[from].push_back({to, cost, static_cast<edge_color>(color)});
                edge_list[to].push_back({from, cost, static_cast<edge_color>(color)});
                E_count++;
            }
        }

        //Name: closest_node
        //Input: none
        //Output: int
        //Description: 
        // Returns unvisited node with smallest current distance.
        int closest_node(){
            int closest_node_indx = -1;
            double min = INF;
            for (int v = 0; v<V_count;v++){
                float new_dist = dist[v];
                if (!visited[v] && new_dist<min){
                    min = new_dist;
                    closest_node_indx = v;
                }
            }
            return closest_node_indx;
        }
        //Name: dikstra_shortest_path
        //Input: none
        //Output: void
        //Description: 
        // Computes shortest paths using Dijkstra’s algorithm with adjacency list.
        void dikstra_shortest_path(){
            construct_data();
            for (int i = 0; i<V_count;i++){
                int closest_n = closest_node();
                if (closest_n == -1){break;}
                visited[closest_n] = true;

                for (edge& e: edge_list[closest_n]){
                    int neighbor = e.index;
                    double weight = e.cost;
                    double new_value = dist[closest_n]+weight;
                    if (new_value<dist[neighbor]){
                        dist[neighbor] = new_value;
                        parent[neighbor] = closest_n;
                    }
                }
            }
        }

        //Name: prim_mst
        //Input: None
        //Output: None
        //Description:
        // Generates a minimum spanning tree using Prim's algorithm
        // and stores the resulting edges in the mst vector.
        void prim_mst(){
            construct_data();
            for (int i = 0; i<V_count;i++){
                int closest_n = closest_node();
                if (closest_n == -1) break;
                visited[closest_n] = true;
                for (edge& e: edge_list[closest_n]){
                    int neighbor = e.index;
                    double weight = e.cost;
                    if (!visited[neighbor]&&(weight < dist[neighbor])){
                        dist[neighbor] = weight;
                        parent[neighbor] = closest_n;
                    }
                }
            }
            for (int i = 1;i<V_count;i++){
                if(parent[i] != -1){
                    mst[parent[i]].push_back(i);
                }
            }

            for(int i = 0; i < V_count; i++){
                if(!visited[i]){
                cout << "Node " << i << " is not in MST\n";
                }
            }


            
        }


};

    //Name: main
    //Input: None
    //Output: int
    //Description:
    // Reads a graph from a file, runs Dijkstra's algorithm,
    // then runs Prim's MST algorithm and prints the results.
int main(){
    Graph g("graph_data.txt");
    g.use_dijkstra();
    g.print_shortest_path(g.get_v_count() - 1);
    cout<<endl;
    g.use_prim();
    g.print_mst();
}

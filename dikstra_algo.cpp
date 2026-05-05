//Date: 5/4/2026
//Name: Mr.lad

//200 words on what I learned:
// During this assignment, I learned how to design and implement a graph as an abstract data type in C++.
// I chose an adjacency matrix representation because the graph densities (20% and 40%) are not very sparse,
// and the matrix allows constant-time edge lookup and simpler implementation.
//
// I also gained a better understanding of Dijkstra’s algorithm. Implementing it from scratch helped me see
// how the algorithm repeatedly selects the closest unvisited node and relaxes its neighbors to update
// shortest paths. Using the distance, visited, and parent arrays made it clear how shortest paths are built
// and how they can be reconstructed.
//
// Another important concept I learned was how randomness affects graph generation. By using a probability
// function based on edge density, I observed that the number of edges varies around an expected value rather
// than being exact. This demonstrated how Monte Carlo simulation works and why averaging results is useful.
//
// I also improved my debugging skills, especially when handling edge cases like disconnected graphs and
// maintaining correct edge counts. Overall, this assignment strengthened my understanding of graph algorithms
// and their practical implementation.


//Program Description:
// Graph Monte Carlo + Dijkstra Implementation
// This program implements an undirected weighted graph using an adjacency matrix.
// Edges are randomly generated using a probability-based density parameter and
// assigned weights in the range [1, 10].
// The program uses Dijkstra's algorithm (O(V^2) implementation) to compute shortest
// paths from node 0 to all other nodes. It then computes the average shortest path
//l ength over multiple randomly generated graphs for different densities (0.2 and 0.4).
// Unreachable nodes are excluded from the average.


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <climits>

using namespace std;



class Graph{
    public:
        //Name: Graph
        //Input: density value (double)
        //Output: none (constructor)
        //Description: 
        // Initializes graph, generates random edges, and sets up data arrays.
        Graph(double density_value = 0.2){
            set_density(density_value);
            gen_graph();
            construct_data();
        }


        //Name: set_density
        //Input: double value
        //Output: none
        //Description: 
        // Sets probability of edge creation in graph generation.
        void set_density(double value){
            density = value;
        }


        //Name: vertices_amount
        //Input: none
        //Output: int
        //Description: 
        // Returns number of vertices in graph.       
        int vertices_amount(){
            return V_count;
        }


        //Name: edge_amount
        //Input: none
        //Output: int
        //Description: 
        // Returns number of edges in graph.
        int edge_amount(){
            return E_count;
        }



        //Name: has_edge
        //Input: two vertex indices (int x, int y)
        //Output: bool
        //Description: 
        // Checks if an edge exists between two nodes.
        bool has_edge(int x,int y){
            return (graph_matrix[x][y] > 0);
        }


        //Name: set_edge_value
        //Input: (int x, int y, double value)
        //Output: void
        //Description: 
        // Sets edge weight. Updates edge count if edge is created or removed.
        // 0 means no edge.
        void set_edge_value(int x_pos,int y_pos,double value){
            if (graph_matrix[x_pos][y_pos] == 0){E_count++;}
            graph_matrix[x_pos][y_pos] = value;
            if (graph_matrix[x_pos][y_pos] == 0){E_count--;}
        }


        //Name: get_edge_value
        //Input: (int x, int y)
        //Output: double
        //Description: 
        // Returns weight of edge between two nodes.
        double get_edge_value(int x_pos,int y_pos){
            return  graph_matrix[x_pos][y_pos];
        }


        //Name: use_dijkstra
        //Input: none
        //Output: void
        //Description: 
        // Runs Dijkstra shortest path algorithm.
        void use_dijkstra(){
            dikstra_shortest_path();
        }


        //Name: print_shortest_path
        //Input: int (target node, optional)
        //Output: void
        //Description: 
        // Recursively prints shortest path from source to target.
        void print_shortest_path(int parent_indx = V_count -1){
            if (parent_indx != -1){
            print_shortest_path(parent[parent_indx]);
            cout <<" :" << parent_indx;
            }
        }


        //Name: get_shortest_dist_avg
        //Input: none
        //Output: double
        //Description: 
        // Returns the average shortest distance from source to each node.
        double get_shortest_dist_avg(){
            double sum = 0.0;
            int viable_edges = 0;
            for(int i = 1; i<V_count;i++){
                if (dist[i]!=INF){
                    sum+=dist[i];
                    viable_edges++;
                }
            }

            return (sum/viable_edges);
        }


        //Name: print_path_distance
        //Input: none
        //Output: void
        //Description: 
        // Prints shortest path distance.
        void print_path_distance(){
            cout<<get_shortest_dist_avg()<<endl;
        }

        //Name: unreachable_dist
        //Input: none
        //Output: double
        //Description: 
        // Returns the distance value in which the node is unreachable.
        double unreachable_dist(){
            return INF;
        }


        //Name: is_connected
        //Input: none
        //Output: bool
        //Description: 
        // Checks if all nodes are reachable from node 0 using graph traversal.
        bool is_connected(){
            int old_size = 0,c_size = 0;
            bool close[V_count];
            bool open[V_count];
            for(int i = 0; i < V_count;++i){
                open[i] = close[i] = false;
            }
            open[0] = true;

            //add to close
            while(c_size < V_count){
                for(int i = 0; i<V_count; ++i){
                    old_size = c_size;
                    if(open[i] && (close[i] == false)){
                        close[i] = true;
                        c_size++;
                        //add to open
                        for (int j = 0; j<V_count;++j){
                            open[j] = open[j] || (graph_matrix[i][j]>0);
                        }
                    }

                }
                if (c_size == old_size){break;}
            }
            return(c_size == V_count);
        }

    private:
        static const int V_count = 50;
        static constexpr double INF = 1e10;
        int E_count = 0;
        double density = 0.2;
        double graph_matrix[V_count][V_count];
        bool visited[V_count];
        double dist[V_count];
        int parent[V_count];


        //Name: prob
        //Input: none
        //Output: double
        //Description: 
        // Generates random probability in range [0,1].
        double prob(){
            return (static_cast<double>(rand()) / RAND_MAX);
        }

        //Name: gen_distance
        //Input: none
        //Output: double
        //Description: 
        // Generates random edge weight in range [1,10].
        double gen_distance(){
            return ((static_cast<double>(rand())/RAND_MAX)*9 + 1);
        }


        //Name: gen_graph
        //Input: none
        //Output: void
        //Description: 
        // Generates random undirected weighted graph using density parameter.
        void gen_graph(){
            for(int i = 0; i < V_count; ++i){
                for (int j = i; j < V_count; ++j){
                    if(i == j){graph_matrix[i][j] = 0;}
                    else if (prob()<density){
                        graph_matrix[i][j] = graph_matrix[j][i] =  gen_distance();
                        E_count++;
                    }
                    //checks only the upper indexes 
                    //so it cant be changed by a later iteration
                    else{
                        graph_matrix[i][j] = graph_matrix[j][i] = 0;
                    }
                }
            }
        }


        //Name: construct_data
        //Input: none
        //Output: void
        //Description: 
        // Initializes visited, distance, and parent arrays.
        void construct_data(){
            for(int i = 0; i < V_count; ++i){
                visited[i] = false;
                dist[i] = INF;
                parent[i] = -1;
            }
            dist[0] = 0;
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
                if (!visited[v] && dist[v]<min){
                    min = dist[v];
                    closest_node_indx = v;
                }
            }
            return closest_node_indx;
        }


        //Name: dikstra_shortest_path
        //Input: none
        //Output: void
        //Description: 
        // Computes shortest paths using Dijkstra’s algorithm (O(V^2)).
        void dikstra_shortest_path(){
            for (int i = 0; i<V_count;i++){

                int closest_n = closest_node();
                if (closest_n == -1){break;}
                visited[closest_n] = true;

                for (int v = 0; v<V_count;v++){
                    if (graph_matrix[closest_n][v] > 0){
                        double new_value = dist[closest_n]+graph_matrix[closest_n][v];
                        if (new_value<dist[v]){
                            dist[v] = new_value;
                            parent[v] = closest_n;
                        }
                    }
                }
            }
        }
};
        



//Name: test_graph_methods
//Input: none
//Output: void
//Description: 
// Tests basic graph functionality and prints results.
void test_graph_methods(){
    Graph g;
    g.use_dijkstra();
    cout<<"Vertices amount: "<<g.vertices_amount()<<endl;
    cout<<"Graph connected is: "<<g.is_connected()<<endl;
    cout <<"Edges in graph: "<<g.edge_amount()<<endl;
    cout<<"has edge between 3 and 4: "<<g.has_edge(3,4)<<" with value of: "<<g.get_edge_value(3,4)<<endl;
    g.set_edge_value(3,4,10);
    cout<<"has edge between 3 and 4: "<<g.has_edge(3,4)<<" with value of: "<<g.get_edge_value(3,4)<<endl;
    cout <<"Edges in graph: "<<g.edge_amount()<<endl;


}



//Name: handle_graph
//Input: double density
//Output: void
//Description: 
// Runs Monte Carlo simulation of graphs with inputed density,
// and computes average shortest path.
void handle_graph(double dens){
    cout << "### Testing Dikstra shortest path when density is: "<<dens<<" ###\n\n";
    int graphs_count = 50;
    int viable_graphs = 0;
    double distance_sum = 0;

    for (int i = 0; i<graphs_count;i++){
        Graph g(dens);
        g.use_dijkstra();
        g.print_path_distance();
        g.print_shortest_path();
        double dist = g.get_shortest_dist_avg();
        if (dist != g.unreachable_dist()){
            distance_sum += g.get_shortest_dist_avg();
            viable_graphs++;
        }
        cout<<endl;
    }
    if(viable_graphs>0){
        double avg_dis = distance_sum/viable_graphs;
        cout<<"avg_dis: "<< avg_dis <<"\n\n";
    }
    else{cout<<"no viable graphs!\n\n";}
}


//Name: main
//Input: none
//Output: int
//Description: 
// Runs simulations for different densities and tests graph functions.
int main(){
    srand(time(0));
    double test_density = 0.2;
    handle_graph(test_density);

    test_density = 0.4;
    handle_graph(test_density);

    test_graph_methods();
    return 0;
}
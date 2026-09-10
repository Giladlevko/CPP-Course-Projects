#include<vector>
#include<string>
#include<unordered_map>
#include<map>
#include<algorithm>
#include<unordered_set>
#include<utility>
#include<queue>
#include<list>
#include<iostream>

using namespace std;


//I used this video by William Fiset for help with the Algorithm
//And I found it very useful to understand how it works!
//https://youtu.be/8MpoO2zA2l4?si=YY6ryUyZVQJ45GRo

//this uses concepts such as bubbles and tips to remove the errors!
//Remove all tips and for each bubble only keep the path of the highest weight
//meaning the path that uses the most agreed upon k-mers

class DE_BRUIJN_GRAPH;
class BUBBLE_REMOVER;

struct edge{
    edge(int t,int w):to(t),weight(w),visits_left(w){}
    edge() = default;
    int to;
    unsigned short weight;
    unsigned short visits_left;

    void operator++(){
        weight++;
        visits_left = weight;
    }
};

struct DE_BRUIJN_ROW{
    DE_BRUIJN_ROW(vector<edge>r):row(r){}
    DE_BRUIJN_ROW() = default;
    vector<edge>row;
    edge& operator[](int& i){
        if(i>row.size()){throw std::out_of_range("Column index out of range");}
        return row[i];
    }
    size_t size(){
        return row.size();
    }
    void push_back_edge(const edge& e){
        row.push_back(e);
    }
};



class DE_BRUIJN_GRAPH{
    public:

        DE_BRUIJN_GRAPH(const vector<string>& entries,const int& k){
            create_k_mer_graph(entries,k);

        }

        vector<DE_BRUIJN_ROW>graph;

        vector<int>in_deg,out_deg;

        vector<string> id_to_str;

        DE_BRUIJN_ROW& operator[](const int& v){
            if(v>graph.size()){throw std::out_of_range("Row index out of range");}
            return graph[v];
        }



        void update_edge_degree(){
            in_deg.resize(graph.size(),0);
            out_deg.resize(graph.size(),0);
            int size = graph.size();
            for(int v = 0; v<size; v++){
                for(int j = 0; j<graph[v].size(); j++){
                    int u = graph[v][j].to;
                    in_deg[u]++;
                    out_deg[v]++;
                }
            }
        }


        int find_edge(const int& v,const int& u){
            for(int i = 0; i<graph[v].size(); i++){
                 if(graph[v][i].to == u){
                    return i;
                }
            }
            return -1;
        }

        
        private:

            void create_k_mer_graph(
                const vector<string>& entries,const int& k
            ){
                int count = entries.size();
                unordered_map<string,int>k_mer_count;
                unordered_map<string,int> str_to_id;

                //min amount of times a k-mer can appear in the graph
                //if it appears less than dont add to the graph
                int min_freq = 1;

                //used for initial filtering of rare k-mers because the e-coli genome
                //will create a lot of errors that would make the graph huge
                for(const string& e:entries){
                    for(int i = 0; i<=e.size()-k; i++){
                        string k_mer = e.substr(i,k);
                        k_mer_count[k_mer]++; 
                    }
                }

                for(const string& e:entries){
                    //sliding a window through the entry to get all k-mers
                    for(int i = 0; i<=e.size()-k; i++){
                        //vert size is k-1 so the edge is k long
                        string pre = e.substr(i,k-1);
                        string suff = e.substr(i+1,k-1);

                        //filtering rare k-mers to avoid making the graph huge
                        if(k_mer_count[pre+suff.back()] < min_freq){continue;}

                        int u = get_id(pre,str_to_id);
                        int v = get_id(suff,str_to_id);

                        cout<<u<<"->"<<v<<" = "<<pre<<"->"<<suff<<"\n";

                        //prevent the same edge from being created twice
                        int v_index = find_edge(u,v);
                        if(v_index == -1){
                            add_edge(u,v);
                        }
                        
                        //++ increments the weight and the visits left
                        ++graph[u][v_index];
                    }
                }
                //free up the allocated space
                k_mer_count = {};
                str_to_id = {};

            }


            
            int get_id(
                const string& s,
                unordered_map<string,int>& str_to_id
            ){
                auto it = str_to_id.find(s);
                if(it != str_to_id.end()){
                    return it->second;
                }
                int id = id_to_str.size();
                id_to_str.push_back(s);
                str_to_id[s] = id;
                add_row();
                return id;
            }

            void add_row(const DE_BRUIJN_ROW& row){
                graph.push_back(row);
            }
            void add_row(){
                DE_BRUIJN_ROW row;
                graph.push_back(row);
            }


            //returns edge from a specific v to a specific u
            edge* get_edge(const int& v,const int& u){
                int i = find_edge(v,u);
                if(i != -1){
                    return &graph[v][i];
                }
                throw std::out_of_range("Edge not found!");

                return nullptr;
            }

            void add_edge(const int&v,const int u){
                edge e(u,0);
                graph[v].push_back_edge(e);
            }


};




class BUBBLE_REMOVER{
    public:
        BUBBLE_REMOVER(DE_BRUIJN_GRAPH&g):graph(g),in_deg(g.in_deg),out_deg(g.out_deg){
            graph.update_edge_degree();
        }
    private:
        DE_BRUIJN_GRAPH& graph;
        vector<int>&in_deg;
        vector<int>&out_deg;

        void record_valid_bubble_vertices_cannidates(
            unordered_set<int>&in_cannidates,unordered_set<int>&out_cannidates
        ){
            int size = in_deg.size();
            for(int v = 0; v<size; v++){
                if(in_deg[v]>=2){
                    in_cannidates.insert(v);
                }
                if(out_deg[v]>=2){
                    out_cannidates.insert(v);
                }
            }
        }

        bool disjoint_paths(const vector<int>&a,const vector<int>&b,const int& vert_count){
            vector<bool>visited(vert_count,false);
            int a_size = a.size();
            int b_size = b.size();
            for(const int& u:a){
                visited[u] = true;
            }
            //remark the shared v and w as false
            visited[a[0]] = visited[a.back()] = false;
            for(const int u:b){
                if(visited[u]){return false;}
            }
            return true;
        }

        void find_paths(
            const int& curr,
            vector<int>&path,
            vector<bool>&visited,
            unordered_set<int>&in_cannidates,
            unordered_map<int,vector<vector<int>>>& to_w_paths,
            const int& max_depth
        ){
            //depth is in edges count not vertex count
            int depth = path.size()-1;
            auto it = in_cannidates.find(curr);
            if(depth>0 &&  it != in_cannidates.end()){
                to_w_paths[curr].push_back(path);
                //we dont return here because we might hit another w later
                //before reaching the depth limit
            }

            if(depth == max_depth){return;}

            for(int i = 0; i<graph[curr].size(); i++){
                const int& u = graph[curr][i].to;
                if(visited[u]){continue;}
                visited[u] = true;
                path.push_back(u);

                find_paths(u,path,visited,in_cannidates,to_w_paths,max_depth);

                //we track back here so we could use this
                //vertex for another path to some other w
                path.pop_back();
                visited[u] = false;
            }
        }

        double get_path_weight(const vector<int>&path){
            int size = path.size();
            int edge_count = size-1;
            double total_weight = 0.0;
            for(int i = 0; i<size-1; i++){
                const int& v = path[i];
                const int& u = path[i+1];
                int j = graph.find_edge(v,u);
                if(j != -1){
                    total_weight += graph[v][j].weight;
                }
            }
            return total_weight / edge_count;
        }
        // need to continue refactoring the bubble removal


};




void remove_path(const vector<int>&path, vector<vector<int>>&graph){
    int size = path.size();
    for(int i = 0; i< size-1; i++){
        const int& u = path[i];
        const int& v = path[i+1];
        auto it = find(graph[u].begin(),graph[u].end(),v);
        if(it != graph[u].end()){
            graph[u].erase(it);
        }
    }
}


int pop_valid_bubbles(
    vector<vector<int>>&graph,
    unordered_set<int>&in_cannidates,unordered_set<int>&out_cannidates,
    const int& max_depth,
    const map<pair<int,int>,int>& edge_weight
){
    int bubble_count = 0;
    int vert_count = graph.size();
    
    for(const int& v:out_cannidates){
        //keeps track af all paths from v to some w (an in_candidate)
        unordered_map<int,vector<vector<int>>> to_w_paths;

        vector<bool>visited(vert_count,false);
        visited[v] = true;
        vector<int>path = {v};
       
        //records all paths from v to w in the to_w_paths
        find_paths(graph,v,path,visited,in_cannidates,to_w_paths,max_depth);

        //for each pair of paths from v to some w we check
        //if all the inward nodes are different from each other
        //(if the two paths are disjoint) if they are then a bubble exist
        //so we count it
        for(const auto& w_paths:to_w_paths){
            //w_paths.second is the paths from v to w
            int path_count = w_paths.second.size();

            //if the path is only two it means its not a bubble but extra paths from v-u
            //which we need int the assembler
            if(path_count == 2){continue;}

            for(int i = 0; i<path_count; i++){
                for(int j = i+1; j<path_count; j++){
                    if(disjoint_paths(w_paths.second[i],w_paths.second[j],vert_count)){
                        double weight_i = get_path_weight(w_paths.second[i],edge_weight);
                        double weight_j = get_path_weight(w_paths.second[j],edge_weight);
                        const vector<int>&path_to_remove = weight_i > weight_j ? w_paths.second[j] : w_paths.second[i];
                        
                        /*
                        cout<<"bubble path removed:\n";
                        for(const int&v:path_to_remove){
                            cout<<v<<" ";
                        }
                        cout<<"\n";
                        //*/

                        remove_path(path_to_remove,graph);
                        bubble_count++;
                    }
                }
            }
        }
    }
    return bubble_count;
}

int remove_bubbles(vector<vector<int>>&graph,const int& max_depth,const map<pair<int,int>,int>& edge_weight){
    int vert_count = graph.size();
    vector<int>in_count(vert_count,0),out_count(vert_count,0);
    count_in_out_degree(graph,in_count,out_count);
    unordered_set<int>in_cannidates,out_cannidates;
    record_valid_bubble_vertices_cannidates(in_count,out_count,in_cannidates,out_cannidates);
    return pop_valid_bubbles(graph,in_cannidates,out_cannidates,max_depth,edge_weight);
}



queue<int> find_tip_candidates(const vector<vector<int>>&graph){
    queue<int>tips;
    int vert_count = graph.size();
    for(int i = 0; i<vert_count; i++){
        if(graph[i].empty()){
            tips.push(i);
        }
    }
    return tips;
}

void remove_edge(vector<vector<int>>&graph,int u,int v){
    auto it = find(graph[u].begin(),graph[u].end(),v);
    if(it != graph[u].end()){
        graph[u].erase(it);
    }
}


bool tip_is_error(
    const vector<vector<int>>&graph,
    const vector<int>&path,
    const int& tip_end,
    const map<pair<int,int>,int>&edge_weight,
    const bool& reversed,
    const vector<int>& in,const vector<int>&out
){
    float tip_weight = 0;
    float tip_end_total_weight = 0;

    //the max percentage that a tip can be counted as not an error
    float max_tip_weight_diff = 0.2;

    cout<<"tip_end: "<<tip_end<<"\n";

    //path.push_back(tip_end);
    for(int i = 0; i<path.size()-1; i++){
        cout<<path[i]<<"->"<<path[i+1]<<"\n";
        if(!reversed){
            tip_weight+=edge_weight.at({path[i],path[i+1]});
        }
        else{
            tip_weight+=edge_weight.at({path[i+1],path[i]});
        }
    }


    //tip_end_total_weight
    //in edges
    for(int v = 0; v<graph.size(); v++){
        if(v==tip_end){continue;}
        for(const int&u:graph[v]){
            if(u==tip_end){

                cout<<"v->tip_end: "<<v<<"->"<<u<<"\n";

                if(!reversed){
                    tip_end_total_weight+=edge_weight.at({v,u});
                }
                else{
                    tip_end_total_weight+=edge_weight.at({u,v});
                }
                    
            }
        }
    }
    //out edges
    for(const int& u:graph[tip_end]){

        cout<<"tip_end->u: "<<tip_end<<"->"<<u<<"\n";

        if(!reversed){
            tip_end_total_weight+=edge_weight.at({tip_end,u});
        }
        else{
            tip_end_total_weight+=edge_weight.at({u,tip_end});
        }
            
    }

    float avrg_weight = tip_end_total_weight/(in[tip_end]+out[tip_end]);

    cout<<"avrg_weight: "<<avrg_weight<<" tip_weight: "<<tip_weight<<"\n";

    if(tip_weight < max_tip_weight_diff * avrg_weight){

        cout<<"tip is error!\n";

        return true;
    }

    cout<<"tip is not error!\n";

    return false;
}


void remove_tip_path(
    int curr,
    vector<int>& in,vector<int>&out,
    vector<vector<int>>&graph,vector<vector<int>>&r_graph,
    queue<int>&tips,
    int& tips_removed,
    const int& max_tip_size,
    const map<pair<int,int>,int>&edge_weight,
    const bool& reversed = false
){
    //since the graph changes I need to make sure
    //that what I had in the queue is still valid
    if(in[curr] !=0 || out[curr] == 0){return;}

    vector<int> path;

    cout<<"curr path: ";

    path.push_back(curr);
    while(out[curr] == 1 && in[curr] <= 1 && path.size()<=max_tip_size+1){
        path.push_back(graph[curr][0]);

        cout<<curr<<" ";

        curr = graph[curr][0];
    }

    cout<<"\n";

    int edge_count = path.size()-1;
    if(edge_count>0 && edge_count <= max_tip_size && (in[curr] > 1 || out[curr] > 1 || out[curr] == 0)){

        if(!tip_is_error(graph,path,curr,edge_weight,reversed,in,out)){return;}

        for(int i = 0; i<edge_count; i++){
            //remove edge u->v
            int u = path[i];
            int v = path[i+1];
            remove_edge(graph,u,v);
            remove_edge(r_graph,v,u);
            in[v]--;
            out[u]--;
            tips_removed++;

            cout<<u<<"->"<<v<<" removed\n";

            //add new tip to queue if the removal of the current one made another
            if(in[v] == 0 && out[v] > 0){
                tips.push(v);
            }

        }
    }
}



vector<vector<int>> create_reverse_graph(const vector<vector<int>>& graph){
    int vert_count = graph.size();
    vector<vector<int>> r_graph(vert_count);
    for(int v = 0; v<vert_count; v++){
        for(const int& u:graph[v]){
            r_graph[u].push_back(v);
        }
    }
    return r_graph;
}


int remove_tips(vector<vector<int>>&graph, const int& max_tip_size,const map<pair<int,int>,int>&edge_weight){
    int vert_count = graph.size();
    vector<vector<int>> r_graph = create_reverse_graph(graph);
    vector<int> in(vert_count,0),out(vert_count,0);
    vector<int>& r_in = out;
    vector<int>& r_out = in;
    count_in_out_degree(graph,in,out);

    queue<int>tips = find_tip_candidates(r_graph);
    queue<int> r_tips = find_tip_candidates(graph);

    int tips_removed = 0;

    while( !r_tips.empty() || !tips.empty() ){

        if(!tips.empty()){
            int tip = tips.front();
            tips.pop();
            remove_tip_path(tip,in,out,graph,r_graph,tips,tips_removed,max_tip_size,edge_weight);
        }
        if(!r_tips.empty()){
            int tip = r_tips.front();
            r_tips.pop();

            //cout<<"backward tip: "<<tip<<"\n";
            bool reversed = true;
            remove_tip_path(tip,r_in,r_out,r_graph,graph,r_tips,tips_removed,max_tip_size,edge_weight,reversed);
        }
    }
    return tips_removed;
}





pair<int,int> find_start_edge(
    const vector<vector<int>>&graph,
    const vector<int>&in_count,const vector<int>&out_count,
    const vector<vector<bool>>&visited
){
    for(int v = 0; v<graph.size(); v++){
        if(out_count[v] > 0 && !(out_count[v] == 1 && in_count[v] == 1)){
            cout<<"graph["<<v<<"].size(): "<<graph[v].size()<<"\n";
            for(int i = 0; i<graph[v].size(); i++){
                cout<<"potential start edge: "<<v<<"->"<<graph[v][i]<<"\n";
                if(!visited[v][i]){

                    cout<<"new start edge: "<<v<<"->"<<graph[v][i]<<"\n";

                    return {v,i};
                }
            }
        }
    }
    return {-1,-1};
}

vector<string> find_contigs(vector<vector<int>>&graph,const vector<string>&id_to_str){
    int size = graph.size();
    vector<int>in_count(size,0);
    vector<int>out_count(size,0);
    count_in_out_degree(graph,in_count,out_count);
    vector<string>contigs;

    vector<vector<bool>>visited(size);
    for(int i = 0; i<size; i++){
        visited[i].resize(graph[i].size(),false);
    }

    //linear contigs starting from a start node that 
    //has out > 0 and !(in == 1 && out == 1)
    while(true){
        pair<int,int> start_edge = find_start_edge(graph,in_count,out_count,visited);
        const int& v = start_edge.first;
        const int& u_index = start_edge.second;

        if(v == -1){break;}

        visited[v][u_index] = true;
        int u = graph[v][u_index];

        //cout<<"starting edge: "<<v<<"->"<<u;

        string contig = id_to_str[v] + id_to_str[u].back();
        int curr = u;
        while(in_count[curr] == 1 && out_count[curr] == 1){
            int next = graph[curr][0];

            //cout<<"->"<<next;

            visited[curr][0] = true;
            contig += id_to_str[next].back();
            curr = next;
        }
        contigs.push_back(contig);

        //cout<<"\n";
    }

    //independent cyclic contigs
    for(int v = 0; v<size; v++){
        for(int i = 0; i<graph[v].size(); i++){
            if(visited[v][i]){continue;}
            visited[v][i] = true;
            int target = graph[v][i];
            string contig = id_to_str[v] + id_to_str[target].back();
            int curr = target;
            while(curr != v){
                int next = graph[curr][0];
                visited[curr][0] = true;
                contig += id_to_str[next].back();
                curr = next;
            }
            contigs.push_back(contig);
        }
    }
    return contigs;
}


void sequence_genome(
    vector<vector<int>>&graph,const int& k,
    const map<pair<int,int>,int>&edge_weight,
    const vector<string>&id_to_str
){
    //need to remove tips first to not mistake them with bubbles
    remove_tips(graph,2*k,edge_weight);
    remove_bubbles(graph,2*k,edge_weight);

    vector<string> contigs = find_contigs(graph,id_to_str);
    for(int i = 0; i<contigs.size(); i++){
        cout<<">CONTIG"<<i+1<<"\n"<<contigs[i]<<"\n";
    }
}


int main(){
    int k = 3;
    vector<string>entries;
    string entry;
    int count;
    cin>>count;
    for(int i = 0; i<count; i++){
        cin>>entry;
        int pos_1 = entry.find('|');
        if(pos_1 == string::npos){
            entries.push_back(entry);
        }
        else{
            int pos_2 = entry.find('|',pos_1+1);
            string r1 = entry.substr(0,pos_1);
            string r2 = entry.substr(pos_1+1,pos_2-pos_1-1);
            entries.push_back(r1);
            entries.push_back(r2);
        }
        
    }
    vector<string>id_to_str;
    map<pair<int,int>,int>edge_weight;
    vector<vector<int>>graph = create_k_mer_graph(entries,k,id_to_str,edge_weight);
    sequence_genome(graph,k,edge_weight,id_to_str);

}
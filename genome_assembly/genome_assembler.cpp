#include<vector>
#include<string>
#include<cstring>
#include<unordered_map>
#include<algorithm>
#include<unordered_set>
#include<utility>
#include<queue>
#include<iostream>

using namespace std;


//I used this video by William Fiset for help with the Algorithm
//And I found it very useful to understand how it works!
//https://youtu.be/8MpoO2zA2l4?si=YY6ryUyZVQJ45GRo

//this uses concepts such as bubbles and tips to remove the errors!
//Remove all tips and for each bubble only keep the path of the highest weight
//meaning the path that uses the most agreed upon k-mers

struct STRING_REF{
    cstring* s;
    int start;
    int length;
    bool operator==(const STRING_REF&other){
        if(length != other.length){return false;}
        return(
            std::memcmp(str->data() + start, other.str->data() + other.start, length) == 0
        );
    }
};


struct STRING_REF_HASHER{
    //The djbj2 hash function
    size_t operator()(const STRING_REF&ref){
        size_t hash = 5381;
        const char* ptr = ref.str->data() + ref.start;
        for(int i = 0; i<ref.length; i++){
            hash += ((hash<<5) + hash) + static_cast<unsigned char>(ptr[i]);
        }
        return hash;
    }
}


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
    edge& operator[](int i){
        if(i>row.size()){throw std::out_of_range("Column index out of range");}
        return row[i];
    }
    size_t size(){
        return row.size();
    }
    void push_back_edge(const edge& e){
        row.push_back(e);
    }
    void erase(const int& i){
        row.erase(row.begin()+i);
    }
};



class DE_BRUIJN_GRAPH{
    public:

        DE_BRUIJN_GRAPH(const vector<string>& entries,const int& k){
            create_k_mer_graph(entries,k);

        }
        DE_BRUIJN_GRAPH() = default;

        vector<DE_BRUIJN_ROW>graph;

        vector<int>in_deg,out_deg;

        vector<string> id_to_str;

        DE_BRUIJN_ROW& operator[](int v){
            if(v>graph.size()){throw std::out_of_range("Row index out of range");}
            return graph[v];
        }



        void update_edge_degree(){
            in_deg.assign(graph.size(),0);
            out_deg.assign(graph.size(),0);
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

        void remove_edge(const int& v, const int& u){
            int i = find_edge(v,u);
            if(i==-1){return;}
            graph[v].erase(i);
            in_deg[u]--;
            out_deg[v]--;
        }

        size_t size(){
            return graph.size();
        }

        void remove_tips_and_bubbles(int max_depth){

            TIP_REMOVER tip_remover(*this,max_depth);
            tip_remover.remove_tips();

            BUBBLE_REMOVER bubble_remover(*this,max_depth);
            bubble_remover.remove_bubbles();
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
                            v_index = graph[u].size() - 1;
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


            //DE BRUIJN GRAPH's private classes

            class BUBBLE_REMOVER{
                public:
                    BUBBLE_REMOVER(DE_BRUIJN_GRAPH&g,int depth):
                    graph(g),in_deg(g.in_deg),out_deg(g.out_deg),max_depth(depth){}

                    ~BUBBLE_REMOVER() = default;


                    int remove_bubbles(){
                        graph.update_edge_degree();
                        unordered_set<int>in_cannidates,out_cannidates;
                        record_valid_bubble_vertices_cannidates(in_cannidates,out_cannidates);
                        return pop_valid_bubbles(in_cannidates,out_cannidates);
                    }


                private:
                    DE_BRUIJN_GRAPH& graph;
                    vector<int>&in_deg;
                    vector<int>&out_deg;
                    int max_depth;

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

                    bool disjoint_paths(const vector<int>&a,const vector<int>&b){
                        vector<bool>visited(graph.size(),false);
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
                        unordered_map<int,vector<vector<int>>>& to_w_paths
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

                            find_paths(u,path,visited,in_cannidates,to_w_paths);

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
                    void remove_path(const vector<int>&path){
                        int size = path.size();
                        for(int i = 0; i< size-1; i++){
                            const int& u = path[i];
                            const int& v = path[i+1];
                            graph.remove_edge(u,v);
                        }
                    }

                    int pop_valid_bubbles(
                        unordered_set<int>&in_cannidates,unordered_set<int>&out_cannidates
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
                            find_paths(v,path,visited,in_cannidates,to_w_paths);

                            //for each pair of paths from v to some w we check
                            //if all the inward nodes are different from each other
                            //(if the two paths are disjoint) if they are then a bubble exist
                            //so we count it
                            for(const auto& w_paths:to_w_paths){
                                //w_paths.second is the paths from v to w
                                int path_count = w_paths.second.size();

                                for(int i = 0; i<path_count; i++){
                                    for(int j = i+1; j<path_count; j++){
                                        if(disjoint_paths(w_paths.second[i],w_paths.second[j])){
                                            double weight_i = get_path_weight(w_paths.second[i]);
                                            double weight_j = get_path_weight(w_paths.second[j]);
                                            const vector<int>&path_to_remove = weight_i > weight_j ? w_paths.second[j] : w_paths.second[i];
                                            
                                            /*
                                            cout<<"bubble path removed:\n";
                                            for(const int&v:path_to_remove){
                                                cout<<v<<" ";
                                            }
                                            cout<<"\n";
                                            //*/

                                            remove_path(path_to_remove);
                                            bubble_count++;
                                        }
                                    }
                                }
                            }
                        }
                        return bubble_count;
                    }

            };







            class TIP_REMOVER{
                public:
                    TIP_REMOVER(DE_BRUIJN_GRAPH&g,int max_tip):
                    graph(g),in_deg(g.in_deg),out_deg(g.out_deg),max_tip_size(max_tip){}

                    void remove_tips(){
                        graph.update_edge_degree();
                        while(true){
                            int forward_removed = remove_forward_tips();
                            int backward_removed = remove_backward_tips();
                            if(forward_removed + backward_removed == 0){
                                break;
                            }
                        }

                    }

                private:
                    DE_BRUIJN_GRAPH& graph;
                    vector<int>&in_deg;
                    vector<int>&out_deg;
                    int max_tip_size;


                    //backward tips are tips that have 0 out degree
                    //therefore we need to trace them from a node with
                    //at least 2 out - this would mean it might lead to
                    //a dead-end tip
                    void find_backward_tip_origin(queue<int>&tips){
                        int vert_count = graph.size();
                        for(int v = 0; v<vert_count; v++){
                            if(out_deg[v]>=2){
                                tips.push(v);
                            }
                        }
                    }

                    //forward tips are tips that start with a node
                    //that has 0 in degree. Not all of these will be
                    //tips but they are the place to start tracing
                    void find_forward_tip_origin(queue<int>&tips){
                        int vert_count = graph.size();
                        for(int v = 0; v<vert_count; v++){
                            if(in_deg[v]==0){
                                tips.push(v);
                            }
                        }
                    }

                    int remove_forward_tips(){
                        queue<int>tips;
                        find_forward_tip_origin(tips);
                        int tips_removed_this_round = 0;
                        while(!tips.empty()){
                            int tip = tips.front();
                            tips.pop();
                            tips_removed_this_round += forward_tip_removal(tip,tips);
                        }
                        return tips_removed_this_round;
                    }

                    int remove_backward_tips(){
                        int tips_removed_this_round = 0;
                        queue<int>tips;
                        find_backward_tip_origin(tips);
                        while(!tips.empty()){
                            int tip = tips.front();
                            tips.pop();
                            tips_removed_this_round += backward_tip_removal(tip);
                        }
                        return tips_removed_this_round;
                    }
                        
                    

                    int forward_tip_removal(const int& tip_start,queue<int>&tips){
                        //since the graph changes I need to make sure
                        //that what I had in the queue is still valid
                        //meaning the forward tip indeed has an in degree
                        //that is 0 and out that is 1
                        if(in_deg[tip_start] != 0 || out_deg[tip_start] != 1){return 0;}

                        int curr = graph[tip_start][0].to;
                        vector<int>path = {tip_start,curr};
                        
                        build_tip_path(path,curr);

                        const int&tip_end = path.back();

                        int edge_count = path.size()-1;
                        int tips_removed = 0;
                        if(edge_count>0 && (in_deg[tip_end] > 1 || out_deg[tip_end] > 1 || out_deg[tip_end] == 0)){

                            if(!tip_is_error(path)){return 0;}

                                remove_tip(path);
                                tips_removed++;
                                //add new tip to queue if the removal of the current one made another
                                if(in_deg[tip_end] == 0 && out_deg[tip_end] > 0){
                                    tips.push(tip_end);
                                }

                        }
                        return tips_removed;
                    }

                    int backward_tip_removal(const int& tip_origin){
                        //since the graph changes I need to make sure
                        //that what I had in the queue is still valid
                        //meaning the origin still has out >= 2
                        int tips_removed = 0;

                        if(out_deg[tip_origin] < 2){return tips_removed;}

                        //need to look at all the edges that could be tips
                        for(int i = graph[tip_origin].size()-1; i>=0; i--){

                            int curr = graph[tip_origin][i].to;
                            if(in_deg[curr] != 1){continue;}
                            vector<int> path = {tip_origin,curr};

                            build_tip_path(path,curr);

                            bool removed_tip = false;

                            const int&tip_end = path.back();
                            if(out_deg[tip_end] == 0 && in_deg[tip_end] == 1){
                                
                                if(!tip_is_error(path)){continue;}

                                remove_tip(path);

                                tips_removed++;
                            }
                            
                        }
                        return tips_removed;
                    }
                        

                    void build_tip_path(vector<int>&path,int curr){
                        /*
                        cout<<"curr path: "<<path[0]<<" "<<curr<<" ";
                        //*/
                        while(out_deg[curr] == 1 && in_deg[curr] == 1 && path.size() <= max_tip_size){
                            curr = graph[curr][0].to;
                            /*
                            cout<<curr<<" ";
                            //*/
                            path.push_back(curr);
                        }
                        /*
                        cout<<"\n";
                        //*/
                    }

                    void remove_tip(const vector<int>&path){
                        int edge_count = path.size()-1;
                        for(int i = 0; i<edge_count; i++){
                            const int& u = path[i];
                            const int& v = path[i+1];
                            graph.remove_edge(u,v);
                        }
                    }


                    bool tip_is_error(const vector<int>&tip){

                        const int& origin = tip[0];

                        float threshold = 0.2;

                        //get tip average weight
                        int tip_total_weight = 0;
                        for(int i = 0; i<tip.size()-1; i++){
                            const int& u = tip[i];
                            const int& v = tip[i+1];
                            int j = graph.find_edge(u,v);
                            if(j != -1){
                                tip_total_weight += graph[u][j].weight;
                            }
                        }
                        float tip_average_weight = static_cast<float>(tip_total_weight) / (tip.size()-1);

                        //get weight of main path (max weight of the out edges of the origin)
                        int main_path_weight = 0;
                        for(int i = 0; i<graph[origin].size(); i++){
                            const int& edge_weight = graph[origin][i].weight;
                            main_path_weight = max(main_path_weight,edge_weight);
                        }
                        //the tip is an error if weight is below threshold relative to main
                        return ( tip_average_weight < main_path_weight * threshold );

                    }
            };


};







class GENOME_ASSEMBLER{
    public:
        GENOME_ASSEMBLER(vector<string>&r):reads(r){
            graph = DE_BRUIJN_GRAPH(reads,k_mer_size);
            graph.remove_tips_and_bubbles(2*k_mer_size);
        }

        void assemble_genome(){
            graph.update_edge_degree();
            vector<string>contigs;
            find_contigs(contigs);
            for(int i = 0; i<contigs.size(); i++){
                cout<<">CONTIG"<<i+1<<"\n"<<contigs[i]<<"\n";
            }
        }

    private:

        const int k_mer_size = 3;
        
        DE_BRUIJN_GRAPH graph;

        vector<string>&reads;

        void find_start_edge(int& u,int& j){
            for(int v = 0; v<graph.size(); v++){
                if(graph.out_deg[v] > 0 && !(graph.out_deg[v] == 1 && graph.in_deg[v] == 1)){
                    /*
                    cout<<"graph["<<v<<"].size(): "<<graph[v].size()<<"\n";
                    //*/
                    for(int i = 0; i<graph[v].size(); i++){
                        /*
                        cout<<"potential start edge: "<<v<<"->"<<graph[v][i]<<"\n";
                        //*/
                        if(graph[v][i].visits_left>0){
                            /*
                            cout<<"new start edge: "<<v<<"->"<<graph[v][i]<<"\n";
                            //*/
                            graph[v][i].visits_left--;
                            u = v; j = i;
                            return;
                        }
                    }
                }
            }
        }

        void find_contigs(vector<string>&contigs){

            //linear contigs starting from a start node that 
            //has out > 0 and !(in == 1 && out == 1)
            const vector<string>&id_to_str = graph.id_to_str;
            while(true){
                int v = -1,u_index = -1;
                find_start_edge(v,u_index);
                if(v == -1){break;}
                const int&u = graph[v][u_index].to;
                //*
                 cout<<"starting edge: "<<v<<"->"<<u;
                //*/
                contigs.push_back("");
                contigs.back() = id_to_str[v] + id_to_str[u].back();
                int curr = u;
                while(graph.in_deg[curr] == 1 && graph.out_deg[curr] == 1){
                    const int& next = graph[curr][0].to;
                    graph[curr][0].visits_left--;
                    //*
                    cout<<"->"<<next;
                    //*/
                    contigs.back() += id_to_str[next].back();
                    curr = next;
                }
                //*
                cout<<"\n";
                //*/
            }

            //independent cyclic contigs
            for(int v = 0; v<graph.size(); v++){
                for(int i = 0; i<graph[v].size(); i++){

                    if( graph[v][i].visits_left <=0 ){continue;}

                    graph[v][i].visits_left--;

                    const int& target = graph[v][i].to;
                    contigs.push_back("");
                    contigs.back() = id_to_str[v] + id_to_str[target].back();

                    int curr = target;

                    while(curr != v){
                        const int& next = graph[curr][0].to;
                        graph[curr][0].visits_left--;
                        contigs.back() += id_to_str[next].back();
                        curr = next;
                    }
                }
            }
        }
};





int main(){

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
    GENOME_ASSEMBLER assembler(entries);
    assembler.assemble_genome();

}
#include<vector>
#include<string>
#include<cstring>
#include<unordered_map>
#include<algorithm>
#include<unordered_set>
#include<utility>
#include<queue>
#include<list>
#include<cstdint>
#include<iostream>

//#include<fstream>


//#include <windows.h>
//#include <psapi.h>

using namespace std;


//I used this video by William Fiset for help with the Algorithm
//And I found it very useful to understand how it works!
//https://youtu.be/8MpoO2zA2l4?si=YY6ryUyZVQJ45GRo

//this uses concepts such as bubbles and tips to remove the errors!
//Remove all tips and for each bubble only keep the path of the highest weight
//meaning the path that uses the most agreed upon k-mers

struct STRING_REF{
    STRING_REF(const string*s,int st,int len):str(s),start(st),length(len){}
    STRING_REF() = default;
    const string* str;
    uint16_t start;
    uint16_t length;
    bool operator==(const STRING_REF&other)const{
        if(length != other.length){return false;}
        if(str == other.str && start == other.start){return true;}
        return(
            std::memcmp(str->data() + start, other.str->data() + other.start, length) == 0
        );
    }
    const char& operator[](size_t i)const{
        return(str->data()[start+i]);
    }
    void print_last_char()const{
        cout.write(str->data()+start+length-1,1);
    }
    friend ostream& operator<<(ostream&out,const STRING_REF& ref);
};


ostream& operator<<(ostream&out,const STRING_REF& ref){
    if(ref.str && ref.length>0){
        out.write(ref.str->data() + ref.start, ref.length);
    }
    return out;
}


struct STRING_REF_HASHER{
    //The murmur3 hash function
    size_t operator()(const STRING_REF&ref)const{
        size_t hash = 14695981039346656037ULL;
        const char* ptr = ref.str->data() + ref.start;
        for(int i = 0; i<ref.length; i++){
            hash ^= static_cast<size_t>(ptr[i]);
            hash *= 1099511628211ULL;
        }
        hash ^= hash>>33;
        hash *= 0xff51afd7ed558ccdULL;
        hash ^= hash>>33;
        hash *= 0xc4ceb9fe1a85ec53ULL;
        hash ^= hash>>33;
        return hash;
    }
};


struct NODE_DATA{
    NODE_DATA(int i= -1,uint16_t c = 0):id(i),count(c){}
    int id;
    uint16_t count;
};


class K_MER_BIT_MAP{
    public:
        K_MER_BIT_MAP(uint16_t k_size):k_mer_len(k_size){}
        K_MER_BIT_MAP() = default;

        typedef pair<uint64_t,uint64_t> K_MER_128;
        static constexpr size_t NOT_FOUND = static_cast<size_t>(-1);

        void process_reads(const vector<string>&reads){
            uint64_t high_len = k_mer_len/2;
            uint64_t prefix_shift = (2 * high_len) - 4; 
            for(uint64_t prefix = 0; prefix<16; prefix++){
                vector<K_MER_128> chunk;
                chunk.reserve(7000000);
                for(const string& e:reads){
                    if(e.size()<k_mer_len){
                        continue;
                    }
                    K_MER_128 current_k_mer = encode_str_to_bit(STRING_REF(&e,0,k_mer_len));
                    if((current_k_mer.first>>prefix_shift) == prefix){
                        chunk.push_back(current_k_mer);
                    }
                    for(size_t i = k_mer_len; i<e.size(); i++){
                        current_k_mer = append_char_to_k_mer(current_k_mer,e[i]);
                        if((current_k_mer.first>>prefix_shift) == prefix){
                            chunk.push_back(current_k_mer);
                        }
                    }
                }
                
                sort_k_mer(chunk);
                clean_bit_arr(chunk);

                if(prefix == 0){
                    bool restart_needed = tune_settings();
                    if(restart_needed){
                        reset();
                        process_reads(reads);
                        return;
                    }
                }

                arr.insert(arr.end(),chunk.begin(),chunk.end());
            }
            
        }

        int16_t get_min_freq(){
            return min_freq;
        }

        size_t size()const{
            return arr.size();
        }

        string str_at(const size_t& i)const{
            if(i>=arr.size()){
                cout<<"size: "<<arr.size()<<" i: "<<i;
                throw std::out_of_range("Bit index out of range");
            }
            return decode_bit_to_str(i);
        }

        void print_str_at(const size_t& i,ostream&out)const{
            if(i>=arr.size()){
                cout<<"size: "<<arr.size()<<" i: "<<i;
                throw std::out_of_range("Bit index out of range");
            }
            print_bit_as_str(i,out);

        }

        K_MER_128 operator[](const size_t& i)const{
            return bit_at(i);
        }

        K_MER_128 bit_at(const size_t& i)const{
            if(i>=arr.size()){
                throw std::out_of_range("Bit index out of range");
            }
            return arr[i];
        }

        void sort_k_mer(vector<K_MER_128>&a){
            sort(a.begin(),a.end());
        }

        size_t find(const STRING_REF&ref)const{
            K_MER_128 bit = encode_str_to_bit(ref);
            auto it = lower_bound(arr.begin(),arr.end(),bit);
            if(it != arr.end() && *it == bit){
                return (distance(arr.begin(),it));
            }
            else{
                return NOT_FOUND;
            }
        }

        size_t end()const{
            return NOT_FOUND;
        }

        uint16_t count_at(size_t i){
            if(i>=arr.size()){
                throw std::out_of_range("Bit index out of range");
            }
            return count_arr[i];
        }

        uint16_t count_of_k_mer(const STRING_REF&ref){
            size_t i = find(ref);
            if(i != NOT_FOUND){
                return count_arr[i];
            }
            else{
                return 0;
            }
        }

        void print_back_char_at(size_t i,ostream&out)const{
            if(i>=arr.size()){
                throw std::out_of_range("Bit index out of range");
            }
            //since I want the last char I
            //use the low side and I shift by 0
            //like in (00 10 11) I go to 11 
            //transform it to its base (T in this case) and print it
            out<<get_base(arr[i].second,0);
        }

        void print_mem_size(){
            cout<<"\nBIT_MAP_SIZE: "<<
            (
                sizeof(vector<K_MER_128>) + sizeof(K_MER_128) * arr.size()+
                sizeof(vector<uint8_t>) + sizeof(uint8_t) * count_arr.size()
            ) / (1024.0 * 1024) <<" MB\n";
        }

        uint16_t get_k_len(){
            return k_mer_len + 1;
        }

    private:
        uint16_t k_mer_len;
        //must be in this order because of how I encoded the bit
        string bases = "ACGT";
        vector<K_MER_128> arr;
        vector<uint8_t>count_arr;
        int16_t min_freq = 2;

        static const uint8_t big_k_mer = 50;
        static const uint8_t low_k_mer = 20;
        static const uint8_t low_coverage = 15;

        size_t gene_vol = 0;
        size_t unique_gene = 0;


        void reset(){
            arr.clear();
            count_arr.clear();
            gene_vol = 0;
            unique_gene = 0;
        }

        

        K_MER_128 encode_str_to_bit(const STRING_REF&ref)const{
            uint64_t low = 0;
            uint64_t high = 0;
            uint16_t half_len = k_mer_len/2;
            for(uint16_t i = 0; i<k_mer_len; i++){
                uint64_t base = get_bit_id(ref[i]);
                
                //this shifts the high/low by 2 bits to the left
                //adding 00 to the right which we populate with base
                //so if we start with G then add T it'll be like this:
                //10 -> 1000 -> 1011
                if(i < half_len){
                    high = (high << 2) | base;
                }
                else{
                    low = (low << 2) | base;
                }
            }
            return{high,low};
        }

        string decode_bit_to_str(const size_t& k_mer_indx)const{
            uint16_t half_len = k_mer_len / 2;
            uint16_t second_half_len = k_mer_len - half_len;
            string result(k_mer_len,'\0');

            for(uint16_t i = 0; i<half_len; i++){
                //the shift needed is how many bits away
                //I am from the far right. this is calculated by 
                //how many bits I shifted it to get to that pos beforehand
                //so for 00 10 01 11 lets say I want 10
                //the shift is 4 -> 2 *( half_len (4) - 1 - i (1) )
                uint16_t shift = 2 * (half_len-1-i);
                result[i] = get_base(arr[k_mer_indx].first, shift);
            }
            for(uint16_t i = 0; i<second_half_len; i++){
                //same thing as before just for the low
                //part of the pair instead of the high
                uint16_t shift = 2 * (second_half_len-1-i);
                result[half_len+i] = get_base(arr[k_mer_indx].second, shift);
            }
            return result;
        }

        //same logic as the decode to str but Here I just print it
        void print_bit_as_str(const size_t& k_mer_indx,ostream&out)const{
            uint16_t half_len = k_mer_len / 2;
            uint16_t second_half_len = k_mer_len - half_len;

            for(uint16_t i = 0; i<half_len; i++){
                uint16_t shift = 2 * (half_len-1-i);
                out<<get_base(arr[k_mer_indx].first, shift);
            }
            for(uint16_t i = 0; i<second_half_len; i++){
                uint16_t shift = 2 * (second_half_len-1-i);
                out<<get_base(arr[k_mer_indx].second, shift);
            }
        }


        char get_base(const uint64_t& half_k_mer,const uint16_t& shift = 0)const{
            //shift that bit by shift amount to the right
            //making it at the far right where I get rid of everything
            //above the first 2 bits i.e every thing bigger than 3
            //giving me the index of the base from what I encoded it
            //i.e 10 = 2 = G which matches the bases' order
            uint16_t base_index = (half_k_mer >> shift) & 3;
            return bases[base_index];
        }

        K_MER_128 append_char_to_k_mer(K_MER_128 k_mer,const char& c){
            uint16_t high_len = k_mer_len / 2;
            uint16_t low_len = k_mer_len - high_len;

            //creates 00111...111 so when I do & mask the right most will be removed
            uint64_t high_mask = (1ULL << (2*high_len)) -1;
            uint64_t low_mask = (1ULL << (2*low_len)) -1;

            uint64_t base = get_bit_id(c);

            //gets the first two bits that now because
            //of appending to low need to be moved to back of high
            uint64_t shifted_from_low = (k_mer.second >> (2*(low_len-1))) & 3;
            //appends the new base to the left of low and removes the 2 extra at the right
            k_mer.second = ((k_mer.second << 2) | base) & low_mask;
            //appends the 2 extra from low to the left of high and removes the two exta at the right
            k_mer.first = ((k_mer.first << 2) | shifted_from_low) & high_mask;
            return k_mer;
        }

        uint64_t get_bit_id(const char& c)const{
            uint64_t id = 0;
            switch(c){
                case 'A': id = 0; break;
                case 'C': id = 1; break;
                case 'G': id = 2; break;
                case 'T': id = 3; break;
                default: id = 0;break;
            }
            return id;
        }

        double coverage_of_k(){
            return ( gene_vol / static_cast<double>(unique_gene) );
        }

        bool tune_settings(){
            double CK = coverage_of_k();
            double target_ck = k_mer_len > 30 ? 12.0 : 6.0;
            if( CK >= target_ck){
                //already good coverage no need to change anything
                return false;
            }
            else if(k_mer_len > low_k_mer){
                min_freq = 2;
                int next_k_len = k_mer_len - 5;
                int min_k = low_k_mer;
                k_mer_len = max(min_k,next_k_len);
            }
            else if(min_freq != 1){
                min_freq = 1;
                k_mer_len = low_k_mer;
            }
            else{
                //ck is low but the best we can do so return false;
                return false;
            }
            //cout<<"CHANGE NEEDED - chosen settings are: K len = "<<k_mer_len<<" min freq = "<<min_freq<<"\n";
            return true;
        }


        void estimate_min_freq(const vector<K_MER_128>&a){
            if(k_mer_len >21){min_freq = 1;}
            else{
                size_t size = a.size();
                
                size_t singles = 0;
                size_t all = 0;
                for(size_t i = 0; i<size; ){
                    size_t j = i+1;
                    while(j<size && a[i] ==  a[j]){
                        j++;
                    }
                    if( (j-i) == 1 ){
                        singles++;
                    }
                    all++;
                    i = j;
                }
                double ratio = static_cast<double>(singles)/all;
                if(ratio < 0.35){
                    min_freq = 1;
                }
                else{
                    min_freq = 2;
                }
            }
            cout<<"MIN_FREQ ESTIMATED TO BE: "<<min_freq<<"\n";
        }


        void clean_bit_arr(vector<K_MER_128>&a){
            if(min_freq == -1){
                estimate_min_freq(a);
            }
            size_t write_index = 0;
            size_t size = a.size();
            for(size_t i = 0; i<size; ){
                size_t j = i+1;
                while(j<size && a[i] == a[j]){
                    j++;
                }
                uint16_t freq = j-i;

                if(freq >= 2){
                    gene_vol += freq;
                    unique_gene++;
                }

                if(freq >= min_freq){
                    a[write_index] = a[i];
                    write_index++;
                    count_arr.push_back(freq);
                }
                i = j;
            }
            a.resize(write_index);
            a.shrink_to_fit();
        }
        

};


struct K_MER_SLOT{
    K_MER_SLOT():occupied(false){}
    STRING_REF key;
    NODE_DATA val;
    bool occupied;
};
class FLAT_K_MER_MAP{
    public:
        FLAT_K_MER_MAP(size_t m):max_size(m),bucket(m){}
        NODE_DATA& operator[](const STRING_REF&ref){
            return find_or_insert(ref);
        }

        K_MER_SLOT* find(const STRING_REF&ref){
            size_t slot_index = hasher(ref) % max_size;
            size_t start = slot_index;
            while(bucket[slot_index].occupied){
                
                if(bucket[slot_index].key == ref){
                    return &bucket[slot_index];
                }
                (slot_index = slot_index + 1) % max_size;
                if(slot_index == start){
                    break;
                }
            }
            return nullptr;
        }

        void print_mem_size(){
            cout<<"\nFLAT K_MER MAP SIZE: "<<
            (
                (sizeof(vector<K_MER_SLOT>) + (sizeof(K_MER_SLOT) * max_size))
                / (1024.0 * 1024)
            ) << " MB\n";
        }

    private:
        size_t max_size;
        vector<K_MER_SLOT>bucket;
        STRING_REF_HASHER hasher;

        NODE_DATA& find_or_insert(const STRING_REF&ref){
            size_t slot_index = hasher(ref) % max_size;
            if(bucket[slot_index].occupied){
                if(bucket[slot_index].key == ref){
                    return bucket[slot_index].val;
                }
                else{
                    return find_next_open_or_matching_slot(slot_index,ref);
                }
            }
            else{
                return create_slot(slot_index,ref);
            }
        }
        NODE_DATA& find_next_open_or_matching_slot(
            size_t i,const STRING_REF&ref
        ){
            int j = (i+1) % max_size;
           while(bucket[i].occupied){
                if(j == i){
                    cout<<"no more space overwriting slot\n";
                    break;
                }
                if(bucket[i].key == ref){
                    return bucket[i].val;
                }
                j = (j+1) % max_size;
                
            }
            return create_slot(j,ref);
        }
        NODE_DATA& create_slot(size_t i,const STRING_REF&ref){
            bucket[i].occupied = true;
            bucket[i].key = ref;
            return bucket[i].val;
        }

        

};


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
        if(i>=row.size()){throw std::out_of_range("Column index out of range");}
        return row[i];
    }
    size_t size()const{
        return row.size();
    }
    void push_back_edge(const edge& e){
        row.push_back(e);
    }
    void erase(const int& i){
        row.erase(row.begin()+i);
    }
    bool empty()const{
        return row.empty();
    }
};


template<typename K, typename V, typename H>
void print_unordered_map_mem_size(const unordered_map<K,V,H>&map){
    int node_overhead = 32;
    cout<<"\nmap size: "<<
    (
        sizeof(unordered_map<K,V,H>) + (map.bucket_count()*sizeof(void*)) +
        (map.size() * (sizeof(K) + sizeof(V) + node_overhead)) + sizeof(H)
    )/(1024.0*1024)
    <<" MB\n";
}



class DE_BRUIJN_GRAPH{
    public:

        DE_BRUIJN_GRAPH(const vector<string>& entries,const int& k){
            create_k_mer_graph(entries,k);
            
        }
        DE_BRUIJN_GRAPH() = default;
        
        vector<DE_BRUIJN_ROW>graph;
        
        vector<int>in_deg,out_deg;
        
        K_MER_BIT_MAP id_to_str;
        uint16_t true_k_len;
        
        DE_BRUIJN_ROW& operator[](int v){
            if(v>=graph.size()){throw std::out_of_range("Row index out of range");}
            return graph[v];
        }
        
        
        
        void update_edge_degree(){
            in_deg.assign(graph.size(),0);
            out_deg.assign(graph.size(),0);
            size_t size = graph.size();
            for(size_t v = 0; v<size; v++){
                for(size_t j = 0; j<graph[v].size(); j++){
                    size_t u = graph[v][j].to;
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
        
        size_t size()const{
            return graph.size();
        }

        size_t get_total_edges(){
            size_t total = 0;
            for(int i = 0; i<graph.size(); i++){
                total += graph[i].size();
            }
            return total;
        }
        
        void remove_tips_and_bubbles(int max_depth){
            
            TIP_REMOVER tip_remover(*this,max_depth);
            BUBBLE_REMOVER bubble_remover(*this,max_depth);
            unsigned int total = 0;
            while(true){
                unsigned int tips = 0;
                unsigned int bubbles = 0;
                tips += tip_remover.remove_tips();
                bubbles += bubble_remover.remove_bubbles();
                //cout<<"Total tips this round: "<<tips<<" Total bubbles this round: "<<bubbles<<"\n";
                total += tips+bubbles;
                if(tips+bubbles == 0){
                    break;
                }
            }
            //cout<<"Total in all rounds: "<<total<<"\n";
            
        }
    
        
        void print_graph(){
            for(int v = 0; v<graph.size(); v++){
                for(int i = 0; i<graph[v].size(); i++){
                    cout<<v<<"->"<<graph[v][i].to<<" = "
                    <<id_to_str.str_at(v)<<"->"<<id_to_str.str_at(graph[v][i].to)
                    <<" Weight: "<<graph[v][i].weight<<"\n";
                }
            }
        }
        

        void print_graph_mem_size(){
            size_t total = 0;
            for(int i = 0; i<graph.size(); i++){
                total += get_row_mem_size(graph[i]);
            }
            cout<<"\ngraph size: "<<total/(1024.0*1024)<<" MB\n";
        }

    
    private:

        void build_seen_twice_arr(
            const vector<string>& entries,
            vector<bool>&seen_twice,
            const STRING_REF_HASHER& hasher, 
            const size_t& size, const int& k
        ){
            seen_twice.assign(size,false);
            vector<bool>seen_once(size,false);
            for(const string& e:entries){
                if(e.size()<k){continue;}
                for(int i = 0; i<=e.size()-k+1; i++){
                    STRING_REF ref(&e,i,k-1);
                    int pos_index = hasher(ref)%size;
                    if(!seen_once[pos_index]){
                        seen_once[pos_index] = true;
                    }
                    else{
                        seen_twice[pos_index] = true;
                    }
                    
                }
            }
        }
    
        void create_k_mer_graph(
            const vector<string>& entries,const int& k
        ){
            int count = entries.size();

            STRING_REF_HASHER hasher;
            //build_seen_twice_arr(entries,seen_twice,hasher,seen_size,k);
            //size_t map_size = 8000000;
            //FLAT_K_MER_MAP node_map_data(map_size);
            id_to_str = K_MER_BIT_MAP(k-1);


            id_to_str.process_reads(entries);
            true_k_len = id_to_str.get_k_len();
            size_t size = id_to_str.size();
            graph.resize(size);

            
            //used for initial filtering of rare k-mers because the e-coli genome
            //will create a lot of errors that would make the graph huge
            /*for(const string& e:entries){
                if(e.size()<k){continue;}
                for(int i = 0; i<=e.size()-k+1; i++){
                    STRING_REF ref(&e,i,k-1);
                    size_t hashed_val = hasher(ref);
                    size_t pos_in_seen = hashed_val%seen_size;
                    if(seen_twice[pos_in_seen]){
                        node_map_data.push(ref);
                    }
                }
            }
            //clear to save memory
            seen_twice.clear(); seen_twice.shrink_to_fit();
            */

            for(const string& e:entries){
                if(e.size()<k){continue;}
                //sliding a window through the entry to get all k-mers
                for(int i = 0; i<=e.size()-k; i++){

                    //vert size is k-1 so the edge is k long
                    STRING_REF pre(&e,i,k-1);
                    STRING_REF suff(&e,i+1,k-1);

                    //filtering rare k-mers to avoid making the graph huge
                    //size_t pre_i = node_map_data.find(pre);
                    //size_t suff_i = node_map_data.find(suff);
                    //if(pre_i == node_map_data.end() || node_map_data.count_at(pre_i)< min_freq){continue;}
                    //if(suff_i == node_map_data.end() || node_map_data.count_at(suff_i) < min_freq){continue;}

                    
                    size_t u = id_to_str.find(pre);
                    size_t v = id_to_str.find(suff);
                    if(u == id_to_str.end()){continue;}
                    if(v == id_to_str.end()){continue;}

                    
                    //cout<<u<<"->"<<v<<" = "<<pre<<"->"<<suff<<"\n";
                    //

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
        }
        
        
        
        /*int get_id(K_MER_SLOT* ptr){
            if(ptr->val.id != -1){
                return ptr->val.id;
            }
            int id = id_to_str.size();
            //id_to_str.push_back(ptr->key);
            ptr->val.id = id;
            add_row();
            return id;
        }*/
        
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
        

        size_t get_row_mem_size(DE_BRUIJN_ROW& row){
            const vector<edge>& vec = row.row;
            return(sizeof(vector<edge>) + sizeof(edge)*vec.size());
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
                vector<vector<int>> paths_to_remove;
                
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
                                    const vector<int>&path_to_remove = (
                                    weight_i > weight_j ? w_paths.second[j] : w_paths.second[i]);
                                    
                                    /*
                                    cout<<"bubble path removed:\n";
                                    for(const int&v:path_to_remove){
                                    cout<<v<<" ";
                                    }
                                    cout<<"\n";
                                    //*/
                                    paths_to_remove.push_back(path_to_remove);
                                    bubble_count++;
                                }
                            }
                        }
                    }
                }
                for(const vector<int>& path:paths_to_remove){
                    remove_path(path);
                }

               return bubble_count;     
            }
                
                
        };
            
            
            
            

            
            
        class TIP_REMOVER{
                public:
                TIP_REMOVER(DE_BRUIJN_GRAPH&g,int max_tip):
                graph(g),in_deg(g.in_deg),out_deg(g.out_deg),max_tip_size(max_tip){}
                
                int remove_tips(){
                    graph.update_edge_degree();
                    int forward_removed = remove_forward_tips();
                    int backward_removed = remove_backward_tips();
                    return backward_removed + forward_removed;
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
                        bool is_forward = true;
                        if(!tip_is_error(path,is_forward)){return 0;}
                        
                        remove_tip(path);
                        tips_removed+=path.size()-1;
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
                            bool is_forward = false;
                            if(!tip_is_error(path,is_forward)){continue;}
                            
                            remove_tip(path);
                            
                            tips_removed+=path.size()-1;
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
                
                
                bool tip_is_error(const vector<int>&tip,bool is_forward = false){
                    return true;
                    if(graph.id_to_str.get_min_freq() == 2){return true;}
                    int origin_indx = is_forward ? tip.size()-1 : 0;
                    const int& origin = tip[origin_indx];
                    
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
                    return ( tip_average_weight < main_path_weight * threshold);
                    
                }
        };
            
  
};
    

/*
void print_peak_memory(){
    PROCESS_MEMORY_COUNTERS info;
    if(GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info))){
        double peak_mb = info.PeakWorkingSetSize / (1024.0 * 1024.0);
        cout << "\n[PEAK MEMORY] " << peak_mb << " MB\n";
    }
}
//*/




    
    
class GENOME_ASSEMBLER{
    public:
        GENOME_ASSEMBLER(vector<string>&r):reads(r){
            //estimate_k_size();
            graph = DE_BRUIJN_GRAPH(reads,k_mer_size);
            //can remove the reads as I don't need them anymore
            reads.clear(); reads.shrink_to_fit();
            /*
            cout<<"original graph:\n";
            graph.print_graph();
            //*/
            //cout<<"Before tip and bubble removal:\nEdge count: "<<graph.get_total_edges()
            //<<"\nVert count: "<<graph.size()<<"\n";
            graph.remove_tips_and_bubbles(graph.true_k_len);

            /*
            cout<<"\nclean graph:\n";
            graph.print_graph();
            //*/
        }
        
        void assemble_genome(){
            graph.update_edge_degree();
            //ofstream file("genome_assembly/contig_output.txt");
            print_contigs(cout);
            //print_eulerian_path();
            //cout<<"\nfinished!\nEdge count: "<<graph.get_total_edges()<<"\nVert count: "<<graph.size()<<"\n";
            //graph.print_graph_mem_size();
            //graph.id_to_str.print_mem_size();

            //print_peak_memory();
            
        }

        void print_eulerian_path(){
            list<int> path = get_eulerian_path();
            const K_MER_BIT_MAP&id_to_str = graph.id_to_str;
            if(path.empty() || path.front() != path.back()){
                cout<<"0";
                return;
            }
            for(auto it = path.begin(); it!=path.end(); it++){
                //cout<<"it is: "<<*it<<" ";
                id_to_str.print_back_char_at(*it,cout);
            }

        }
        
    private:
        
        int k_mer_size = 51;
        
        DE_BRUIJN_GRAPH graph;
        
        vector<string>&reads;


        void estimate_k_size(){
            size_t gc_count = 0;
            size_t all_bases = 0;
            for(const string&e:reads){
                for(int i = 0; i<e.size(); i++){
                    if(e[i] == 'C' || e[i] == 'G'){
                        gc_count++;
                    }
                    all_bases++;
                }
            }
            double gc_percent = (static_cast<long double>(gc_count) / all_bases) * 100.0;
            if(gc_percent < 30.0 || gc_percent > 70.0){
                k_mer_size = 51;
            }
            else{
                k_mer_size = 21;
            }
            cout<<"KMER SIZE ESTIMATED TO FIT: "<<k_mer_size<<" GC_percent: "<<gc_percent<<"\n";
        }


        
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
                            
                            u = v; j = i;
                            return;
                        }
                    }
                }
            }
        }

        int find_active_edge(int v){
            for(int i = 0; i<graph[v].size(); i++){
                if(graph[v][i].visits_left>0){
                    return i;
                }
            }
            return -1;
        }
        
        void print_contigs(ostream&out){
            size_t contigs_total_length = 0;
            //linear contigs starting from a start node that 
            //has out > 0 and !(in == 1 && out == 1)
            const K_MER_BIT_MAP&id_to_str = graph.id_to_str;
            size_t curr_contig = 1;
            while(true){
                int v = -1,u_index = -1;
                find_start_edge(v,u_index);
                if(v == -1){break;}
                const int&u = graph[v][u_index].to;
                graph[v][u_index].visits_left=0;
                /*
                cout<<"starting edge: "<<v<<"->"<<u;
                //*/
                out<<">CONTIG"<<curr_contig<<"\n";
                curr_contig++;
                id_to_str.print_str_at(v,out);
                id_to_str.print_back_char_at(u,out);
                contigs_total_length+=k_mer_size;
                int curr = u;
                while(
                    graph.in_deg[curr] == 1 && graph.out_deg[curr] == 1 &&
                    !graph[curr].empty() && graph[curr][0].visits_left>0
                ){
                    const int& next = graph[curr][0].to;
                    graph[curr][0].visits_left=0;
                    /*
                    cout<<"->"<<next;
                    //*/
                    id_to_str.print_back_char_at(next,out);
                    contigs_total_length++;
                    curr = next;
                }
                out<<"\n";
            }
            
            //independent cyclic contigs
            for(int v = 0; v<graph.size(); v++){
                for(int i = 0; i<graph[v].size(); i++){
                    
                    if( graph[v][i].visits_left <=0 ){continue;}
                    
                    graph[v][i].visits_left=0;
                    
                    const int& target = graph[v][i].to;
                    out<<">CONTIG"<<curr_contig<<"\n";
                    curr_contig++;
                    id_to_str.print_str_at(v,out);
                    id_to_str.print_back_char_at(target,out);

                    contigs_total_length += k_mer_size;
                    
                    int curr = target;
                    
                    while(curr != v && !graph[curr].empty()){
                        int edge_indx = find_active_edge(curr);
                        if(edge_indx == -1){break;}
                        const int& next = graph[curr][edge_indx].to;
                        graph[curr][edge_indx].visits_left=0;
                        id_to_str.print_back_char_at(next,out);

                        contigs_total_length++;

                        curr = next;
                    }
                    out<<"\n";
                }
            }

            //cout<<"THE TOTAL LENGTH OF ALL CONTIGS IS: "<<contigs_total_length<<"\n";
        }



        bool graph_has_eulerian_path(){
            size_t size = graph.size();
            size_t start_nodes = 0, end_nodes = 0;
            for(int i = 0; i<size; i++){
                //for a path to exist there must be at most
                // a difference of 1 between the in / out degrees
                if ( abs(graph.in_deg[i] - graph.out_deg[i]) > 1){
                    return false;
                }
                else if(graph.out_deg[i] - graph.in_deg[i] == 1){
                    start_nodes++;

                }

                else if(graph.in_deg[i] - graph.out_deg[i] == 1){
                    end_nodes++;
                }

            }
            //either there are no start/end nodes or there are 1 of each
            bool has_path = (start_nodes == 0 && end_nodes == 0) || (start_nodes == end_nodes == 1);
            return has_path;
        }


        void dfs(list<int>&path,int curr){
            while(graph.out_deg.at(curr) > 0){
                //select the next unvisited edge
                //the out_deg is used both to know how much
                //edges I have left to discover 
                //and also to index the next node 
                size_t next_node_index = --graph.out_deg.at(curr);
                size_t next_node = graph[curr][next_node_index].to;
                dfs(path,next_node);
            }
            path.push_front(curr);
        }


        size_t get_start_node(){
            size_t size = graph.size();
            size_t start = 0;
            for(int i = 0; i<size; i++){
                //if a vert has more out than in by 1
                //it is a unique starting node
                if((graph.out_deg[i] - graph.in_deg[i]) == 1){return i;}

                //otherwise just choose some node with an out degree
                //of at least one
                if(graph.out_deg[i]>0){
                    start = i;
                }
            }
            return start;
        }


        list<int> get_eulerian_path(){
            list<int>path;
            graph.update_edge_degree();
            if( !graph_has_eulerian_path() ){
                cout<<"no path!\n";
                return path;
            }
            size_t start_node = get_start_node();

            dfs(path,start_node);

            size_t edge_count = graph.get_total_edges();

            //if we didnt traverse a correct number of verices
            //i.e edge_count+1 than our path doesnt exist
            //therefore we should return an empty one;
            if(path.size() != edge_count+1){path.clear();}

            return path;
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
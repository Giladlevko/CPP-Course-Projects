#include<vector>
#include<string>
#include<unordered_map>
#include<iostream>

using namespace std;

bool is_matching(
    const string& a,const string& b,
    const int& a_start, const int& b_start, 
    const int& len, const double& miss_perc
){
    int max_miss = miss_perc*len;
    int miss_count = 0;
    for(int i = 0; i<len; i++){
        if(a[a_start+i] != b[b_start+i]){
            miss_count++;
        }
        if(miss_count>max_miss){
            return false;
        }
    }
    return true;
}

int get_overlap_with_mismatch(const string& a,const string& b){
    int a_size = a.size();
    int b_size = b.size();
    int min_size = a_size<=b_size ? a_size : b_size;
    //percent of miss-matches allowed
    double miss_perc = 0.05;
    //k = 12 ensures that no incorrect substrings will be calculated as matching
    //because in a gene i.e with only 4 chars there are 4^12 combinations
    // so there is a very low chance they will be unrelated
    int k = 12;
    //if the suffix of 'a' matches the prefix of 'b' then 'a' comes before 'b'
    for(int len = min_size; len>k; len--){
        int a_start = a.size()-len;
        int b_start = 0;
        
        if(is_matching(a,b,a_start,b_start,len,miss_perc)){
            return len;
        }
    }
    return 0;
}

int char_index(const char& c){
    switch(c){
        case 'A':return 0;
        case 'C':return 1;
        case 'G':return 2;
        case 'T':return 3;
        default:return 0;
    }
}


string get_char_consensus(vector<string>&reads,vector<int>&reads_pos,const int& row_count){
    vector<vector<int>>consensus_matrix(row_count,vector<int>(4,0))
    int read_count = reads.size();
    int read_size = reads[0].size();
    for(int i = 0; i<read_count; i++){
        for(const char& c:reads[i]){
            
        }
    }
}


string hamiltonian_greedy(vector<string>&reads){
    int k = 12;
    int read_count = reads.size();
    vector<bool>visited(read_count,false);
    int start = 0;
    int curr = start;
    int last = -1;
    int pos = 0;
    vector<int>reads_pos(read_count,0);
    string genome = reads[curr];
    while(last!=curr){
        visited[curr] = true;
        
        int max_overlap = 0;
        int best_next = -1;
        last = curr;
        for(int i = 0; i<read_count; i++){
            if(visited[i]){continue;}
            int overlap = get_overlap_with_mismatch(reads[curr],reads[i]);
            if(overlap>max_overlap){
                max_overlap = overlap;
                best_next = i;
            }
        }
        //if I found a next match I can update the genome
        if(best_next != -1){
            curr = best_next;
            pos += reads[curr].size() - max_overlap;
            read_count[curr]  = pos;
            genome += reads[curr].substr(max_overlap);
        }
    }
    int final_overlap = get_overlap_with_mismatch(reads[last],reads[start]);
    int row_count = pos+reads[curr].size() - final_overlap;
    genome = genome.substr(0,genome.size()-final_overlap);
    return genome;
}



int main(){
    //it is given that the reads count is 1618
    int count = 1618;
    vector<string>reads(1618);
    for(int i = 0; i<count; i++){
        cin>>reads[i];
    }
    cout<<hamiltonian_greedy(reads);
}
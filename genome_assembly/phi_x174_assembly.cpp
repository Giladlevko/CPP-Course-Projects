#include<vector>
#include<string>
#include<unordered_map>
#include<iostream>

using namespace std;

int get_overlap(const string& a,const string& b){
    int a_size = a.size();
    int b_size = b.size();
    int min_size = a_size<=b_size ? a_size : b_size;
    //if the suffix of 'a' matches the prefix of 'b' then 'a' comes before 'b'
    for(int len = min_size; len>0; len--){
        if(a.compare(a.size()-len,len,b,0,len) == 0){
            return len;
        }
    }
    return 0;
}

unordered_map<string,vector<int>> compute_k_mer_map(vector<string>&reads,const int& k){
    //computes the k-mer map meaning it looks at 
    //the first k letters and stores the read index
    //then we can look at the k letter long suffix
    //of another string and see what read matches
    unordered_map<string,vector<int>>map;
    int read_count = reads.size();
    for(int i = 0; i<read_count; i++){
        string key = reads[i].substr(0,k);
        map[key].push_back(i);
    }
    return map;
}

string hamiltonian_greedy(vector<string>&reads){
    int k = 12;
    unordered_map<string,vector<int>>map = compute_k_mer_map(reads,k);
    int read_count = reads.size();
    vector<bool>visited(read_count,false);
    int start = 0;
    int curr = start;
    int last = -1;
    string genome = reads[curr];
    while(last!=curr){
        visited[curr] = true;
        
        int max_overlap = -1;
        int best_next = -1;
        last = curr;
        for(int i = 0; i<=reads[curr].size() -k; i++){
            string suffix = reads[curr].substr(i,k);
            if(!map.count(suffix)){continue;}
            for(const int& match:map[suffix]){
                if(visited[match]){continue;}
                int overlap = get_overlap(reads[curr],reads[match]);
                if(overlap>max_overlap){
                    max_overlap = overlap;
                    best_next = match;
                }
            }
        }
        
        /*
        turned out to be an unnececary fallback

        but it was supposed to be a fallback for when I didnt find a
        next and there were still unvisited nodes
        if(best_next == -1){
            //if I didnt find an unvisited k large overlap there might be smaller ones
            //so I should get the best one
            for(int i = 0; i<read_count; i++){
                if(visited[i]){continue;}
                int overlap = get_overlap(reads[curr],reads[i]);
                if(overlap>max_overlap){
                    max_overlap = overlap;
                    best_next = i;
                }

            }
        }
        */

        //if I found a next match I can update the genome
        if(best_next != -1){
            curr = best_next;
            genome += reads[curr].substr(max_overlap);
        }
        
    }
    int final_overlap = get_overlap(reads[last],reads[start]);
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



#include<vector>
#include<random>
#include<algorithm>
#include<iostream>

using namespace std;

const long long WIDTH = 200000;
const long long DEPTH = 5;
const long long large_prime = 2147483647;


struct sketch_pair{
    sketch_pair(long long i,long long c):id(i),count(c){}
    sketch_pair() = default;
    long long id;
    long long count;
};

class count_sketch{
    public:
        count_sketch(vector<sketch_pair>&arr){
            count_sketch();
            int size = arr.size();
            for(int i = 0; i<size; i++){
                update(arr[i].id,arr[i].count);
            }

        }
        count_sketch(){
            //init hash values for the approx point query
            sign_a = sign_b = bucket_a = bucket_b = vector<long long>(DEPTH,0);
            init_rows_rand_hash_vars();
            count_matrix = vector<vector<long long>>(DEPTH,vector<long long>(WIDTH,0));
        }

         bool is_larger(const long long& i, const int& threshold){
            return approx_point_query(i)>=threshold;
        }

        void update(
            const long long& i,
            const long long& count
        ){
            for(int row = 0; row<DEPTH; row++){
                int k = get_cell_index(i,row);
                count_matrix[row][k] += count*sign(i,row);
            }

        }


    private:
        vector<long long>sign_a,sign_b;
        vector<long long>bucket_a,bucket_b;
        vector<vector<long long>>count_matrix;

        void init_rows_rand_hash_vars(
        ){
            //make the rng deterministic
            mt19937_64 rng(1000);
            uniform_int_distribution<long long> dist(1,large_prime-1);
            for(int row = 0; row<DEPTH; row++){
                sign_a[row] = dist(rng);
                sign_b[row] = dist(rng);
                bucket_a[row] = dist(rng);
                bucket_b[row] = dist(rng);
            }
        }
        int get_cell_index(
            const long long& k,
            const int& row
        ){
            long long val = (k * bucket_a[row] + bucket_b[row])%large_prime;
            return val%WIDTH;
        }
        int sign(
            const long long& k,
            const int& row
        ){
            long long val = (k * sign_a[row] + sign_b[row])%large_prime;
            int result = val%2 == 0? 1:-1;
            return result;
        }

        long long estimate(
            const long long& i,
            const int& row
        ){
            int k = get_cell_index(i,row);
            return count_matrix[row][k] * sign(i,row);
        }
        long long approx_point_query(
            const long long& index
        ){
            vector<long long>estimates;
            for(int row = 0; row<DEPTH; row++){
                
                estimates.push_back(estimate(index,row));
            }
            //return the median of the estimates
            sort(estimates.begin(),estimates.end());
            return estimates[DEPTH/2];
        }

};


int main(){
    int id_count,threshold;
    cin>>id_count>>threshold;
    count_sketch sketch;
    //good deeds
    for(int i = 0; i<id_count; i++){
        long long id,count;
        cin>>id>>count;
        sketch.update(id,count);
    }
    //bad deeds
    for(int i = id_count; i<id_count*2; i++){
        long long id,count;
        cin>>id>>count;
        sketch.update(id,-1*count);
    }
    int query_count;
    cin>>query_count;
    for(int q = 0; q<query_count; q++){
        long long id;
        cin>>id;
        cout<<sketch.is_larger(id,threshold)<<" ";
    }
}








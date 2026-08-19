#include<vector>
#include<random>
#include<iostream>

using namespace std;

const long long WIDTH = 200000;
const long long DEPTH = 5;
const long long large_prime = 2147483647;


void init_rows_rand_hash_vars(
    vector<int>&sign_a,vector<int>&sign_b,
    vector<int>&bucket_a,vector<int>&bucket_b
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
    const int& row,
    const vector<int>&bucket_a,
    const vector<int>&bucket_b
){
    long long val = (k * bucket_a[row] + bucket_b[row])%large_prime;
    return val%width;
}

int sign(
    const long long& k,
    const int& row,
    const vector<int>&sign_a,
    const vector<int>&sign_b
){
    long long val = (k * sign_a[row] + sign_b[row])%large_prime;
    int result = val%2 == 0? 1:-1;
    return result;
}
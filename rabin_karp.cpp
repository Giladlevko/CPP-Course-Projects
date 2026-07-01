#include<vector>
#include<string>
#include<iostream>
#include<random>
using namespace std;

bool are_equal(string s1,string s2){
    int size = s1.size();
    if (size != s2.size()){
        return false;
    }
    for(int i = 0;i<size;i++){
        if (s1[i]!=s2[i]){
            return false;
        }
    }
    return true;
}

long long poly_hash(const string& s,long long p, long long x){
    long long hash_val = 0;
    for(int i = s.length()-1;i>=0;i--){
        hash_val = (hash_val*x + s[i])%p;
    }
    return hash_val;
}

vector<long long> precompute_hashes(const string& text,const int& pattern_size,const long long& p,const long long& x){
    int text_size = text.size();
    vector<long long> hashes(text_size-pattern_size+1);
    string s = "";
    for(int i = text_size-pattern_size;i<text_size;i++){
        s+=text[i];
    }
    hashes[text_size-pattern_size] = poly_hash(s,p,x);
    long long y = 1;
    for(int i = 1;i<=pattern_size;i++){
        y = (y*x) % p;
    }
    for(int i = text_size-pattern_size-1;i>=0;i--){
        hashes[i] = (x*hashes[i+1]+text[i]-y*text[i+pattern_size])%p;
        hashes[i] = (hashes[i]+p)%p;
    }
    return hashes;
}

vector<int> rabin_karp(const string& text,const string& pattern){
    int text_size = text.size(),pattern_size = pattern.size();

    long long p = 1000000007;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, p-1);
    long long x = distrib(gen);

    vector<int> positions;
    int p_hash = poly_hash(pattern,p,x);

    vector<long long> hashes = precompute_hashes(text,pattern_size,p,x);

    for (int i = 0;i<=text_size-pattern_size;i++){
        if(p_hash == hashes[i]){
            string s ="";
            for(int j = i;j<i+pattern_size;j++){
                s+=text[j];
            }
            if(are_equal(s,pattern)){
                positions.push_back(i);
            }
        }
    }
    return positions;
}

int main(){
    string pattern = "",text = "";
    cin>>pattern>>text;
    vector<int> positions = rabin_karp(text,pattern);
    for(const int& i:positions){
        cout<<i<<" ";
    }
}
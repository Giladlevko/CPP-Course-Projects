#include<vector>
#include<string>
#include<iostream>
using namespace std;

//recieves a string and a suffix array of that string,
// and computes the longest common prefix array of that string


int lcp_of_suffixes(const string& s,const int& i,const int& j,const int& equal_count){
    int lcp = (equal_count>0) ? equal_count : 0;
    int size = s.size();
    while( (i+lcp < size) && (j+lcp < size) ){
        if (s[i+lcp] == s[j+lcp]){
            lcp++;
        }
        else{break;}
    }
    return lcp;
}


vector<int> inverse_suffix_arr(const vector<int>& suffix_arr){
    int size = suffix_arr.size();
    vector<int> pos(size);
    for(int i = 0; i<size; i++){
        pos[suffix_arr[i]] = i;
    }
    for(int i = 0; i<size;i++){
        cout<<pos[i]<<" ";
    }
    cout<<"\n";
    return pos;
}


vector<int> get_lcp_arr(const string& s,const vector<int>& suffix_arr){
    int size = s.size();
    vector<int> lcp_arr(size-1);
    int lcp = 0;
    vector<int> pos_in_order = inverse_suffix_arr(suffix_arr);
    int suffix = suffix_arr[0];
    for(int i = 0; i<size; i++){
        int order_index = pos_in_order[suffix];
        if(order_index == size-1){
            lcp = 0;
            suffix = (suffix+1)%size;
            continue;
        }
        int next_suffix = suffix_arr[order_index+1];
        lcp = lcp_of_suffixes(s,suffix,next_suffix,lcp-1);
        lcp_arr[order_index] = lcp;
        suffix = (suffix+1)%size;
    }
    return lcp_arr;
}


int main(){
    string s;
    cin>>s;
    vector<int>suffix_arr(s.size());
    for(int i = 0; i<s.size(); i++){
        cin>>suffix_arr[i];
    }
    vector<int>lcp_arr = get_lcp_arr(s,suffix_arr);
    for(int i = 0; i<s.size()-1;i++){
        cout<<lcp_arr[i]<<" ";
    }
}
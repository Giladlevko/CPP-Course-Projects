#include<vector>
#include<iostream>
#include<string>

using namespace std;

vector<int> compute_prefix(const string& pattern){
    int size = pattern.size();
    vector<int> border_array(size);
    border_array[0] = 0;
    int curr_border = 0;
    
    for(int i = 1; i < size; i++){
        while(curr_border > 0 && pattern[i] != pattern[curr_border]){
            curr_border = border_array[curr_border-1];
        }
        if(pattern[i] == pattern[curr_border]){
            curr_border++;
        }
        else{
            curr_border = 0;
        }
        border_array[i] = curr_border;
    }
    return border_array;
}

vector<int> kmp_match(const string&pattern,const string&text){
    vector<int>result;

    int pattern_size = pattern.size();
    int text_size = text.size();

    if(pattern_size>text_size){return result;}
    
    string concat_string = pattern + '$' + text;
    int concat_size = concat_string.size();
    
    vector<int> border_array = compute_prefix(concat_string);
    
    for(int i = pattern_size+1; i<concat_size; i++){
        if(border_array[i] == pattern_size){
            result.push_back(i-2*pattern_size);
        }
    }
    return result;
}

int main(){
    string pattern,text;
    cin>>pattern>>text;
    vector<int> matches = kmp_match(pattern,text);
    for(const int&i:matches){
        cout<<i<<" ";
    }
}
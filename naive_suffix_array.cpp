#include<vector>
#include<iostream>
#include<string>
#include<algorithm>

using namespace std;


vector<int> get_suffix_arr_naive(const string &text){
    int size = text.size();
    vector<int> suffix_arr(size);
    for(int i = 0; i<size; i++){
        suffix_arr[i] = i;
    }

    sort(suffix_arr.begin(),suffix_arr.end(),
    [&text](int a, int b){
        //checks if the suffix that starts at a should be before or after the suffix that starts at b
        //if a comes before b then compare will return a negative number
        //if they are the same it'll be 0
        //otherwise it will return positive number 
        //meaning suffix that start in b comes before the one that starts in a
        return text.compare(a,string::npos,text,b,string::npos) < 0;
    });

    return suffix_arr;
}

int main(){
    string text;
    cin>>text;
    vector<int>suffix_arr = get_suffix_arr_naive(text);
    for(const int& i:suffix_arr){
        cout<<i<<" ";
    }
}
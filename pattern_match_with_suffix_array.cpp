#include<vector>
#include<string>
#include<iostream>
#include<utility>

using namespace std;


int char_to_id(const char& c){
    switch(c){
        case '$':return 0;
        case 'A':return 1;
        case 'C':return 2;
        case 'G':return 3;
        case 'T':return 4;
        default: return 0;
    }
}


vector<int> initial_char_sort(const string& s){
    int size = s.size();
    int char_count = 5;//$,A,C,G,T
    vector<int>order(size);
    vector<int>count(char_count,0);
    for(int i = 0; i<size; i++){
        count[char_to_id(s[i])]++;
    }
    for(int i = 1; i<char_count; i++){
        count[i] += count[i-1];
    }
    for(int i = size-1; i>=0; i--){
        int c = char_to_id(s[i]);
        count[c]--;
        order[count[c]] = i;
    }
    /*
    cout<<"init sort ";
    for(int i =0;i<size;i++){cout<<order[i]<<" ";}
    cout<<"\n";
    */
    return order;
}

vector<int> initial_class_compute(const string& s,const vector<int>&order){
    int size = s.size();
    vector<int> classes(size);
    classes[order[0]] = 0;
    for(int i = 1; i<size; i++){

        int cur_char_order = order[i];
        int prev_char_order = order[i-1];

        if( s[cur_char_order] != s[prev_char_order] ){
            classes[cur_char_order] = classes[prev_char_order] + 1;
        }
        else{
            classes[cur_char_order] = classes[prev_char_order];
        }
    }
    /*
    cout<<"init classes ";
    for(int i =0;i<size;i++){cout<<classes[i]<<" ";}
    cout<<"\n";
    */
    return classes;
}

vector<int> sort_doubled(
    const string& s, const int& len,
    const vector<int>&order,
    const vector<int>& classes
){
    int size = s.size();
    vector<int>new_order(size);
    vector<int>count(size,0);
    for(int i = 0; i<size; i++){
        count[classes[i]]++;
    }
    for(int i = 1; i<size; i++){
        count[i] += count[i-1];
    }
    for(int i = size-1; i>=0; i--){
        int start = ((order[i] - len + size) % size);
        int cl = classes[start];
        count[cl]--;
        new_order[count[cl]] = start;
    }
    /*
    cout<<"sort doubled ";
    for(int i =0;i<size;i++){cout<<new_order[i]<<" ";}
    cout<<"\n";
    */
    return new_order;
}

vector<int> update_classes(
    const vector<int>&new_order,
    const vector<int>& classes,
    const int& len
){
    int size = new_order.size();
    vector<int> new_classes(size);
    new_classes[new_order[0]] = 0;
    for(int i = 1; i<size; i++){
        int cur = new_order[i];
        int prev = new_order[i-1];
        int mid = (cur+len)%size;
        int mid_prev = (prev+len)%size;
        if( (classes[cur] != classes[prev]) || (classes[mid] != classes[mid_prev]) ){
            new_classes[cur] = new_classes[prev] + 1;
        }
        else{
            new_classes[cur] = new_classes[prev];
        }
    }
    /*
    cout<<"update classes ";
    for(int i =0;i<size;i++){cout<<new_classes[i]<<" ";}
    cout<<"\n";
    */
    return new_classes;
}

vector<int> build_suffix_arr(const string& s){
    vector<int> order = initial_char_sort(s);
    vector<int> classes = initial_class_compute(s,order);
    int len = 1;
    int size = s.size();
    while(len<size){
        order = sort_doubled(s,len,order,classes);
        classes = update_classes(order,classes,len);
        len = 2*len;
    }
    return order;
}

pair<int,int> pattern_match_with_suffix_array(
    const string& s,const string& pattern,
    const vector<int>&suffix_arr
){
    int size = s.size();
    int min_index = 0;
    int max_index = size;
    while(min_index < max_index){//searches for the first occurrence of match
        int mid_index = (min_index+max_index)/2;
        //does pattern come after the suffix starting in suffix_arr[min_index]?
        if(pattern.compare(0,string::npos, s, suffix_arr[mid_index], pattern.size()) > 0){
            min_index = mid_index + 1;
        }
        else{
            max_index = mid_index;
        }
    }
    int start = min_index;
    max_index = size;
    while(min_index<max_index){//searching for the last occurrence of the match (gives the position after)
        int mid_index = (min_index+max_index)/2;
        //does pattern come before the suffix starting in suffix_arr[min_index]?
        if(pattern.compare(0,string::npos, s, suffix_arr[mid_index], pattern.size()) < 0){
            max_index = mid_index;
        }
        else{
            min_index = mid_index + 1;
        }
    }
    int end = max_index;
    if(start>=end){
        return{-1,-1};
    }
    else{
        return{start,end};
    }
}

void collect_occurences(const string&s,const string&pattern,
     const vector<int>&suffix_arr,vector<bool>&occurrences){
    pair<int,int> range = pattern_match_with_suffix_array(s,pattern,suffix_arr);
    if(range.first == -1){return;}
    for(int i = range.first; i<range.second; i++){
        occurrences[suffix_arr[i]] = true;
    }
}

int main(){
    string s;
    cin>>s;
    int size = s.size();
    vector<int> suffix_arr = build_suffix_arr(s);
    vector<bool>occurrences(size);
    int n;
    cin>>n;
    for(int i = 0; i<n; i++){
        string pattern;
        cin>>pattern;
        collect_occurences(s,pattern,suffix_arr,occurrences);
    }
    for(int i = 0; i<size; i++){
        if(occurrences[i]){
            cout<<i<<" ";
        }
    }
    return 0;
}

#include<vector>
#include<string>
#include<iostream>

using namespace std;

//gets the start index of characters 
// in the sorted version of the text using count-sort
vector<int> get_start_index(const string&text){
    vector<int>count(256);//256 is the max value for any  ASCII character
    vector<int>start_index(256);
    int size = text.size();

    for(int i = 0; i<size; i++){
        count[text[i]]++;
    }

    start_index['$'] = 0;
    start_index['A'] = start_index['$'] + count['$'];
    start_index['C'] = start_index['A'] + count['A'];
    start_index['G'] = start_index['C'] + count['C'];
    start_index['T'] = start_index['G'] + count['G'];
    return start_index;

/*assignment only includes characters A,C,G,T,$
    general version would be: 
    int curr_offset = 0;
    for(int i = 0; i<256 ;i++){
        start_index[i] = curr_offset;
        curr_offset += count[i];
    }*/
}

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

vector<vector<int>> compute_count(const string& bwt_text){
    int size = bwt_text.size();
    vector<int>count(5,0);
    vector<vector<int>>count_by_level(size+1);
    for(int i = 0; i<size; i++){
        count_by_level[i] = count;
        char curr_char = bwt_text[i];
        int index = char_to_id(curr_char);
        count[index]++;

    }
    count_by_level[size] = count;
    return count_by_level;
}

int symbol_count_in_level(const char& symbol,const int& level,const vector<vector<int>>&count_by_level){
    return count_by_level[level][char_to_id(symbol)];
}

int better_BWT_matching(
    const string& bwt_text,string& pattern,
    const vector<int>&start_index,const vector<vector<int>>&count_by_level){
    int size = bwt_text.size();
    int top = 0;
    int bottom = size -1;
    while(top <= bottom){
        if(!pattern.empty()){
            char symbol = pattern.back();
            pattern.pop_back();
            int symbol_count_up_to_top = symbol_count_in_level(symbol,top,count_by_level);
            int symbol_count_up_to_bottom = symbol_count_in_level(symbol,bottom+1,count_by_level);
            int occurences = symbol_count_up_to_bottom - symbol_count_up_to_top;
            if(occurences > 0){
                top = start_index[symbol]+symbol_count_up_to_top;
                bottom = start_index[symbol] + symbol_count_up_to_bottom -1;
            }
            else{
                return 0;
            }
        }
        else{
            return bottom - top +1;
        }
    }
    return 0;
}

int main(){
    string bwt_text;
    cin>>bwt_text;
    vector<int> start_index = get_start_index(bwt_text);
    vector<vector<int>> count_by_level = compute_count(bwt_text);
    int p_count;
    cin>>p_count;
    for(int i =0; i<p_count ;i++){
        string pattern;
        cin>>pattern;
        cout<<better_BWT_matching(bwt_text,pattern,start_index,count_by_level)<<" ";
    }
}
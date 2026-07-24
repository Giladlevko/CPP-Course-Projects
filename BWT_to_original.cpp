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

vector<int> get_right_to_left(const string&bwt_text){
    vector<int> start_of_left = get_start_index(bwt_text);
    vector<int>right_count(256,0);
    int size = bwt_text.size();
    vector<int>right_to_left(size);
    for(int i = 0; i<size ;i++){
        char curr_char = bwt_text[i];
        right_to_left[i] = start_of_left[curr_char] + right_count[curr_char];
        right_count[curr_char]++;
    }
    return right_to_left;

}

string reverse_bwt(string& text){
    vector<int> right_to_left = get_right_to_left(text);
    int size = text.size();
    string result(size,'\0');//init with null char and same size of text
    int curr_index = 0;
    for(int i = 0; i<size;i++){
        if(text[i] == '$'){curr_index = i;break;}
    }
    for(int i = 0; i<size ;i++){
        result[size-i-1] = text[curr_index];
        int left_index = right_to_left[curr_index];
        curr_index = left_index;
    }
    return result;
}

int main(){
    string bwt_text;
    cin>>bwt_text;
    cout<<reverse_bwt(bwt_text);
}
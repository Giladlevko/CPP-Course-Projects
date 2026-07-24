#include<vector>
#include<iostream>
#include<string>
#include<algorithm>

using namespace std;

string get_bwt(string& text){//(naive version) runs in O(n^2*log(n))
    int size = text.size();
    vector<string> string_rotations;
    string_rotations.push_back(text);
    for(int i = 1; i<size; i++){
        string old_rotation = string_rotations.back();
        char last_char =  old_rotation.back();
        old_rotation.pop_back();
        string new_rotation = last_char + old_rotation;
        string_rotations.push_back(new_rotation);
    }
    sort(string_rotations.begin(),string_rotations.end());

    string bwt_string  = "";

    for(int i = 0; i<size; i++){
        bwt_string +=string_rotations[i][size-1];
    }
    return bwt_string;
}


int main(){
    string text;
    cin>>text;
    cout<<get_bwt(text);
}
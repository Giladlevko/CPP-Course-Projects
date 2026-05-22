#include <string>
#include <regex>
#include <iostream>
using namespace std;

int main(){
    string fnames[] = {"ira.txt","data.txt","ira2.txt","a.out"};
    regex txt_regex("[a-z]+\\.txt");
    for(int i = 0; i < 4;i++){
        //print out the string and see if there is a match,
        //if there is it'll print 1 if not 0
        cout<<fnames[i]<<":"<<regex_match(fnames[i],txt_regex)<<endl;
    }
}
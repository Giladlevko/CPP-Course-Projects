#include <iostream>
#include <vector>
using namespace std;

int main(){
    int size;
    cout << "How many ints in data"<<endl;
    cin>> size;
    vector<int> data (size);
    cout << "the contents of data: ";
    for (vector<int>::iterator it = data.begin(); it != data.end(); ++it){
        cin>> *it;
    }   // could use auto instead of vector<int>::iterator
    for (vector<int>::iterator it = data.begin(); it != data.end(); ++it){
        cout<< *it<<endl;
    }

}
#include<map>
#include<unordered_map>
#include<string>
#include<iostream>
using namespace std;

int main(){
    //for these maps the first type is for the key type
    // the second is for the data type
    //The unordered map is more time efficiant and uses a hash map
    //The normal map is less time efficiant and uses a red black tree
    map<unsigned long, string> worker;
    unordered_map<unsigned long,unsigned> payroll;
    unsigned total_pay = 0;
    worker[99567800] = "Harold Fish";
    payroll[99567800] = 67300;
    worker[8567900] = "Philip Fish";
    payroll[8567900] = 87300;
    for (auto p = worker.begin();p!=worker.end();++p){
        //associate (first,second) => (id no, Name)
        cout<<"Name: "<<(*p).second<<"\t id no. "<<(*p).first<<endl;
    }
    for(auto p = payroll.begin();p!=payroll.end();++p){
        total_pay+=(*p).second;
    }
    cout<<"Pyroll Totals "<<total_pay<<"$"<<endl;
}
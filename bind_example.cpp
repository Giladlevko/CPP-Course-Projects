#include <iostream>
#include<functional>
#include<algorithm>
using namespace std;

template<class forwIter>
void print(forwIter first,forwIter last,const char* title){
    cout <<title<<endl;
    while(first!=last){
        cout<<*first++<<"\t";
    }
    cout<<endl;
}

int main(){
    int data[3] = {9,10,11};
    print(data,data+3,"Original values:");
    transform(data,data+3,data,bind2nd(multiplies<int>(),2));
    //with lambda, WHICH IS WAY BETTER AND MORE INTUITIVE:
    transform(data,data+3,data,[](int x){return (x*2);});
    print(data,data+3,"New values (after *4):");
}
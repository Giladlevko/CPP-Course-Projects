#include <iostream>
using namespace std;

long long fib(int n){
    long long old;
    long long f1 = 1;
    long long f2 = 0;
    for(int i = 0; i<n;i++){
        old = f2;
        f2 = f2 + f1;
        f1 = old;
    }
    return f2;
}

int main(){
    int n;
    cin>>n;
    cout<<fib(n);
    return 0;
}
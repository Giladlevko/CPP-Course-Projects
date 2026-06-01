#include <iostream>
using namespace std;

int fib_last_digit(long long n){
    n = n%60;
    int old;
    int f1 = 1;
    int f2 = 0;
    for(int i = 0; i<n;i++){
        old = f2;
        f2 = (f2 + f1)%10;
        f1 = old;
    }
    return f2;
}

int main(){
    long long n;
    cin>>n;
    int result = 0;
    result = fib_last_digit(n);
    cout<<result;
    return 0;
}
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
    long long n,m;
    cin>>m>>n;
    int sum = 0;
    sum = (fib_last_digit(n+2)+9)%10 - (fib_last_digit(m+1)+9)%10;
    
    cout<<(sum+10)%10;
    return 0;
}
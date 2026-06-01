#include <iostream>
using namespace std;


long long pisano(long long m) {
    long long prev = 0;
    long long curr = 1;

    for (long long i = 0; i< m*m; i++) {
        long long next = (prev + curr) % m;
        prev = curr;
        curr = next;
        if (prev == 0 && curr == 1){
            return i + 1;
        }
    }
    return 1;
}

long long fib_mod(long long n,long long m){
    long long period = pisano(m);
    n = n%period;
    long long old;
    long long f1 = 0;
    long long f2 = 1;
    if (n <= 1) return n;
    for(long long i = 2; i<=n;i++){
        old = f2;
        f2 = (f2 + f1)%m;
        f1 = old;
        
    }
    return f2;
}

int main(){
    long long n,m;
    cin>>n>>m;
    cout<<fib_mod(n,m);
    return 0;
}
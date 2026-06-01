#include <iostream>
using namespace std;
//an algorithm to calculate the least common multiplyer efficiantly

int euc_gcd(int a, int b){
    if (b == 0){return a;}
    int a_prime = a % b;
    return euc_gcd(b,a_prime);
}

//using the relationship between lcm and gcd
// lcm(a,b)*gcd(a,b) = a*b
long long lcm(long long a,long long b){
    int g = euc_gcd(a,b);
    return static_cast<long long>((a*b/g));
}

int main(){
    int a,b;
    // input 2 integers
    cin>> a >> b;
    long long result = lcm(a,b);
    //output result
    cout<< result;
}
#include <iostream>
using namespace std;
//an algorithm to calculate the greatest common divisor efficiantly

int euc_gcd(int a, int b){
    //because a = a_prime + b*q then
    // some d that divides a and b would also divide a_prime.
    // Therefore gcd(a,b) = gcd(a_prime,b) = gcd(b,a_prime)
    if (b == 0){return a;}
    int a_prime = a % b;
    return euc_gcd(b,a_prime);
}

int main(){
    int a,b;
    // input 2 integers
    cin>> a >> b;
    int result = euc_gcd(a,b);
    //output result
    cout<< result;
}
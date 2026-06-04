//calculates the min amount of coins needed for a given amount of money
#include<iostream>
using namespace std;

long long min_coins(long long amount){
    return (amount/10 + (amount%10)/5 + amount%5);
}

int main(){
    long long a = 0;
    cin>>a;
    cout << min_coins(a)<<endl;
    return 0;
}
#include<iostream>
#include<vector>
using namespace std;

int change_dp(const long long& money,const vector<int>&coins){
    long long INF = 1e10;
    int coin_size = coins.size();
    vector<int> min_coin_num(money+1,0);
    for(int m = 1;m<=money;m++){
        min_coin_num[m] = INF;
        for(int i=0;i<coin_size;i++){
            if (m>=coins[i]){
                int coin_num = min_coin_num[m-coins[i]]+1;
                if (coin_num<min_coin_num[m]){
                    min_coin_num[m] = coin_num;
                }
            }
        }
    }
    return min_coin_num[money];
}

int main(){
    vector<int>coins = {1,3,4};
    int a = 0;
    cin>>a;
    cout<<change_dp(a,coins);
}
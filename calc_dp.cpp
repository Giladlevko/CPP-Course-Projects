#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

//where operations are +1, *2, *3
void calc_dp(const long long& num){
    vector<int>min_op_num(num+1,0);
    int INF = 1e9;
    for(int n = 2;n<=num;n++){
        min_op_num[n] = min_op_num[n-1]+1;
        if(n%2 == 0){
            min_op_num[n] = min(min_op_num[n],min_op_num[n/2]+1);
        }
        if(n%3==0){
            min_op_num[n] = min(min_op_num[n],min_op_num[n/3]+1);
        }
    }
    cout<<min_op_num[num]<<endl;
    vector<long long> past_num;
    long long curr = num;
    while(curr>=1){
        past_num.push_back(curr);
        if(curr == 1){break;}
        if(curr%3==0 && min_op_num[curr] == min_op_num[curr/3]+1){
            curr/=3;
        }
        else if(curr%2==0 && min_op_num[curr] == min_op_num[curr/2]+1){
            curr/=2;
        }
        else{
            curr-=1;
        }
    }
    for(int n = past_num.size()-1;n>0;n--){
        cout<<past_num[n]<<" ";
    }
}

int main(){
    long long a = 0;
    cin>>a;
    calc_dp(a);
}
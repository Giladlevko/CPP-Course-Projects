#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
using namespace std;

int edit_dist(string A,string B){
    int n = A.size();
    int m = B.size();
    vector<vector<int>> D(n+1,vector<int>(m+1,0));
    for(int i = 1;i<=n;i++){
        D[i][0] = i;
    }
    for(int j = 1;j<=m;j++){
        D[0][j] = j;
    }
    for(int j = 1;j<=m;j++){
        for(int i = 1;i<=n;i++){
            int ins = D[i][j-1]+1;
            int del = D[i-1][j]+1;
            int match = D[i-1][j-1];
            int mismatch = D[i-1][j-1]+1;
            if (A[i-1] == B[j-1]){
                D[i][j] = min(min(ins,del),match);
            }
            else{D[i][j] = min(min(ins,del),mismatch);}
        }
    }
    return D[n][m];
}

int main(){
    string a = "";
    string b = "";
    cin>>a>>b;
    cout<<edit_dist(a,b);
}
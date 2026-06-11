#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
using namespace std;

int lcs2(const vector<long long>& A,const vector<long long>& B){
    int n = A.size();
    int m = B.size();
    vector<vector<int>> lcs(n+1,vector<int>(m+1,0));
    for (int i = 1;i<=n;i++){
        for(int j = 1;j<=m;j++){
            if(A[i-1] == B[j-1]){
                lcs[i][j] = lcs[i-1][j-1] + 1;
            }
            else{lcs[i][j] = max(lcs[i][j-1],lcs[i-1][j]);}
        }
    }
    return lcs[n][m];
}


int lcs3(const vector<long long>& A,const vector<long long>& B,const vector<long long>& C){
    int n = A.size();
    int m = B.size();
    int o = C.size();
    vector<vector<vector<int>>> lcs(n+1,vector<vector<int>>(m+1,vector<int>(o+1,0)));
    for (int i = 1;i<=n;i++){
        for(int j = 1;j<=m;j++){
            for(int k = 1;k<=o;k++){
                if(A[i-1] == B[j-1] && A[i-1] == C[k-1]){
                    lcs[i][j][k] = lcs[i-1][j-1][k-1] + 1;
                }
                else{lcs[i][j][k] = max({lcs[i][j-1][k],lcs[i-1][j][k],lcs[i][j][k-1]});}
            }
        }
    }
    return lcs[n][m][o];
}


int main(){
    int n = 0,m = 0,o = 0;
    vector<long long> a;
    vector<long long> b;
    vector<long long> c;
    cin>>n;
    for(int i = 0;i<n;i++){
        long long k = 0;
        cin>>k;
        a.push_back(k);
    }
    cin>>m;
    for(int i = 0;i<m;i++){
        long long k = 0;
        cin>>k;
        b.push_back(k);
    }
    cin>>o;
    for(int i = 0;i<o;i++){
        long long k = 0;
        cin>>k;
        c.push_back(k);
    }
    cout<<lcs3(a,b,c);
}
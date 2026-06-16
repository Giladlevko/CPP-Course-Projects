#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<utility>
using namespace std;


long long eval(const long long& a,const char& op,const long long& b){

    if (op == '+'){return (a+b);}
    else if (op == '-'){return (a-b);}
    else if (op == '*'){return (a*b);}
    else if (op == '/'){return (a/b);}
    else{return 0;}
        

    
}

pair<long long,long long> min_and_max(
    const int& i,const int& j,const vector<vector<long long>>& maxs,const vector<vector<long long>>& mins,const string& op
){
    long long min_val = 1e10;
    long long max_val = -1e10;
    for(int k = i;k<j;k++){
        long long a = eval(maxs[i][k],op[k],maxs[k+1][j]);
        long long b = eval(maxs[i][k],op[k],mins[k+1][j]);
        long long c = eval(mins[i][k],op[k],maxs[k+1][j]);
        long long d = eval(mins[i][k],op[k],mins[k+1][j]);
        min_val = min({min_val,a,b,c,d});
        max_val = max({max_val,a,b,c,d});
    }
    return{min_val,max_val};
}

long long parentheses(vector<long long>& nums,string& ops){
    int size = nums.size()-1;
    vector<vector<long long>> maxs (size+1,vector<long long>(size+1,0));
    vector<vector<long long>> mins (size+1,vector<long long>(size+1,0));
    for(int i = 1;i<=size;i++){
        mins[i][i] = nums[i];
        maxs[i][i] = nums[i];
    }
    for(int s = 1; s<size;s++){
        for(int i = 1;i<=size-s;i++){
            int j = i+s;
            pair<long long,long long> min_n_max = min_and_max(i,j,maxs,mins,ops);
            mins[i][j] = min_n_max.first;
            maxs[i][j] = min_n_max.second;
        }
    }
    return maxs[1][size];
}

void parse_expr(string& s,vector<long long>& nums,string& ops){

    int size = s.size();
    for(int i = 0;i<size;i++){
        if(i%2 == 0){
            nums.push_back(s[i]-'0');
        }
        else{ops+=s[i];}
    }
    ;
}

int main(){
    vector<long long> nums;
    string ops;
    nums.push_back(0);
    ops+='0';
    string a = "";
    cin>>a;
    parse_expr(a,nums,ops);
    cout<<parentheses(nums,ops);
    return 0;
}
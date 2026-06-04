//outpouts the max salary for a given array of numbers
#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
using namespace std;

class number_string{
    public: 
        number_string(string v):val(v){}
        string val;
};

bool sort_by_string(string& a,string& b){
    return( (a+b) > (b+a));
};

class concat_num{
    public:
        concat_num(int n):num_amount(n){}
        int num_amount;
        vector<string> nums;
        

        vector<string> maximize_sal(){
            sort(nums.begin(),nums.end(),sort_by_string);
            return nums;
        }
        void print_sal(){
            for (string& s:nums){
                cout<<s;
            }
        }
};

int main(){
    int n = 0;
    cin>>n;
    concat_num nums_concat = concat_num(n);
    for(int i = 0; i<n;i++){
        string a = "";
        cin>>a;
        nums_concat.nums.push_back(a);
    }
    nums_concat.maximize_sal();
    nums_concat.print_sal();
    return 0;

}
//calculates the max dot product for 2 given arrays
#include<vector>
#include<iostream>
#include<algorithm>
using namespace std;

class number_arrays{
    public:
        number_arrays(int s):size(s){}
        int size;
        vector<int> arr_1;
        vector<int> arr_2;
        long long max_product(){
            long long result = 0;
            sort(arr_1.begin(),arr_1.end());
            sort(arr_2.begin(),arr_2.end());
            for (int i = 0;i<size;i++){
                result += static_cast<long long>(arr_1[i])*arr_2[i];
            }
            return result;
        }
};

int main(){
    int n = 0;
    cin>>n;
    number_arrays arrays = number_arrays(n);
    for (int i = 0; i<n; i++){
        int p = 0;
        cin>>p;
        arrays.arr_1.push_back(p);
    }
    for (int i = 0; i<n; i++){
        int c = 0;
        cin>>c;
        arrays.arr_2.push_back(c);
    }
    cout<<arrays.max_product();
    return 0;
}
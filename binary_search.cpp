#include<iostream>
#include<vector>
using namespace std;

int bin_serch(vector<long long>& arr,long long& val){
    int left = 0;
    int right = arr.size()-1;
    int result = -1;
    while(right>=left){
        int mid = (right + left)/2;
        if (val==arr[mid]){
            result = mid;
            right = mid-1;
        }
        else if (val>arr[mid]){
            left = mid+1;
        }
        else if (val<arr[mid]){
            right = mid - 1;
        }
    }
    return(result);
}

int main(){
    int n = 0;
    int m = 0;
    vector<long long> sorted_array;
    vector<long long> values;
    cin>>n;
    for (int i = 0; i<n;i++){
        long long a = 0;
        cin>>a;
        sorted_array.push_back(a);
    }
    cin>>m;
    for (int j = 0; j<m;j++){
        long long a = 0;
        cin>>a;
        values.push_back(a);
    }

    for(int k = 0;k<m;k++){
        cout<<bin_serch(sorted_array,values[k])<<" ";
    }

}
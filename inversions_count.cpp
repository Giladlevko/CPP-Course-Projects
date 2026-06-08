#include<iostream>
#include<vector>
using namespace std;

long long merge_and_count(vector<long long>& arr,int left,int mid,int right){
    int i = left;
    int j = mid+1;
    long long inv_count = 0;
    vector<long long>temp;
    while(i<=mid&&j<=right){
        if(arr[i]<=arr[j]){
            temp.push_back(arr[i]);
            i++;
        }
        else{
            temp.push_back(arr[j]);
            inv_count+=mid-i+1;
            j++;
        }
    }
    for(;i<=mid;i++){
        temp.push_back(arr[i]);
    }
    for(;j<=right;j++){
        temp.push_back(arr[j]);
    }
    for(int k = 0;k<temp.size();k++){
        arr[left+k] = temp[k];
    }
    return inv_count;
}




long long merge_sort(vector<long long>&arr,int left,int right){
    long long inv_count = 0;
    if (left <right){
        int m = left + (right - left) / 2;;
        inv_count += merge_sort(arr,left,m);
        inv_count += merge_sort(arr,m+1,right);
        inv_count += merge_and_count(arr,left,m,right);
    }
    return inv_count;
}

int main(){
    int n = 0;
    cin>>n;
    vector<long long> arr;
    for (int i = 0; i<n;i++){
        int a = 0;
        cin>> a;
        arr.push_back(a);
    }
    cout<<merge_sort(arr,0,n-1);
}
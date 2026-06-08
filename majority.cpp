#include<iostream>
#include<vector>
using namespace std;

int majority(vector<long long>& arr){
    int size = arr.size();
    long long canidate = arr[0];
    int count = 0;
    int result = 0;
    for(int i = 0;i<size;i++){
        if (canidate == arr[i]){
            count++;
        }
        else{
            count--;
            if (count == 0){
                canidate = arr[i+1];
            }
        }
    }
    int appearence_count = 0;
    for(int i = 0;i<size;i++){
        if (arr[i]==canidate){
            appearence_count++;
        }
    }
    if (appearence_count>size/2){
        result = 1;
    }

    return result;
}

int main(){
    vector<long long> arr;
    int n;
    cin>>n;
    for(int i = 0;i<n;i++){
        long long a = 0;
        cin>>a;
        arr.push_back(a);
    }
    cout<<majority(arr);
}
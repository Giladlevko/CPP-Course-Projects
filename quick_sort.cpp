#include<vector>
#include<iostream>
#include<utility>
#include<random>
using namespace std;

template <typename T>
void swap(vector<T>& arr,int l,int r){
    T temp = arr[l];
    arr[l] = arr[r];
    arr[r] = temp;
}

template <typename T>
int partition(vector<T>& arr,int l,int r){
    T x = arr[l];
    int j = l;
    for (int i = l+1;i<r;i++){
        if(arr[i]<=x){
            j++;
            swap(arr,j,i);
        }
    }
    swap(arr,l,j);
    return j;
}

template <typename T>
pair<int,int> partition3(vector<T>& arr,int l,int r){
    T x = arr[l];
    int m1 = l;
    int m2 = r;
    for (int i = l+1;i<m2;){
        if(arr[i]<x){
            m1++;
            swap(arr,m1,i);
            i++;
        }
        else if(arr[i]>x){
            m2--;
            swap(arr,m2,i);
        }
        else{i++;}
        
    }
    swap(arr,l,m1);
    return {m1,m2};
}

template <typename T>
void quick_sort(vector<T>& arr,int l = 0,int r = 0){
    while(l<r){
        int mid = l + (r - l) / 2;
        int k = l;
        if ((arr[mid] >= arr[l] && arr[mid] <= arr[r-1]) || (arr[mid] <= arr[l] && arr[mid] >= arr[r-1])) {
            k = mid;
        } 
        else if ((arr[r-1] >= arr[l] && arr[r-1] <= arr[mid]) || (arr[r-1] <= arr[l] && arr[r-1] >= arr[mid])) {
            k = r - 1;
        }
        swap(arr,l,k);
        pair<int,int> m = partition3(arr,l,r);
        if(m.first-l<r-m.second){
            quick_sort(arr,l,m.first);
            l = m.second;
        }
        else{
            quick_sort(arr,m.second,r);
            r = m.first;
        }
    }
}


int main(){
    vector<int> arr;
    int n;
    cin>>n;
    for(int i = 0;i<n;i++){
        int a = 0;
        cin>>a;
        arr.push_back(a);
    }
    quick_sort(arr,0,arr.size());
    for(int i = 0; i<n;i++){
        cout<<arr[i]<<" ";
    }
}
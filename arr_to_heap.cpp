#include<vector>
#include<iostream>
#include<utility>

using namespace std;



int parent(const int& i){
    return((i-1)/2);
}
int right_child(const int& i){
    return(2*i+2);
}
int left_child(const int& i){
    return(2*i+1);
}

void sift_down(int i,vector<long long>& arr,int& swap_count,vector<pair<int,int>>& swaped_nums){
    int size = arr.size();
    int max_indx = i;
    int l = left_child(i);
    int r = right_child(i);
    if (l<size && arr[l]<arr[max_indx]){
        max_indx = l;
    }
    if (r<size && arr[r]<arr[max_indx]){
        max_indx = r;
    }
    if (i != max_indx){
        swaped_nums.push_back({i,max_indx});
        long long temp = arr[i];
        arr[i] = arr[max_indx];
        arr[max_indx] = temp;
        swap_count++;
        sift_down(max_indx,arr,swap_count,swaped_nums);
    }
}


void build_heap(vector<long long>& arr,int& swap_count,vector<pair<int,int>>& swaped_nums){
    int size = arr.size()-1;
    for(int i = size/2;i>=0;i--){
        sift_down(i,arr,swap_count,swaped_nums);
    }
}

int main(){
    vector<pair<int,int>> swaped_nums;
    int swap_count = 0;
    vector<long long> nums;
    int size = 0;
    cin>>size;
    for(int i = 0;i<size;i++){
        long long a = 0;
        cin>>a;
        nums.push_back(a);
    }
    build_heap(nums,swap_count,swaped_nums);
    cout<<swap_count<<"\n";
    for(pair<int,int>& p:swaped_nums){
        cout<<p.first<<" "<<p.second<<"\n";
    }

    return 0;
}
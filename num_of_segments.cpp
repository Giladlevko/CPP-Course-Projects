#include<iostream>
#include<vector>
#include<algorithm>
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

int count_bigger_end(vector<long long>&arr,long long val){
    int left = 0;
    int right = arr.size()-1;
    int result = arr.size();
    while(right>=left){
        int mid = (right + left)/2;
        if (val<=arr[mid]){
            result = mid;
            right = mid-1;
        }
        else if (val>arr[mid]){
            left = mid+1;
        }
    }
    return(arr.size()-result);
}


int main(){
    int s = 0,p = 0;
    vector<long long> starts;
    vector<long long> ends;
    vector<long long>points;
    cin>>s>>p;
    for (int i = 0;i<s;i++){
        long long start = 0,end = 0;
        cin>>start>>end;
        starts.push_back(start);
        ends.push_back(end);
    }
    for(int i = 0; i<p;i++){
        long long point = 0;
        cin>>point;
        points.push_back(point);
    }
    sort(starts.begin(),starts.end());
    sort(ends.begin(),ends.end());
    for(int i = 0;i<p;i++){
        int count = count_bigger_end(ends,points[i]) - count_bigger_end(starts,points[i]+1);
        cout<<count<<" ";
    }
}
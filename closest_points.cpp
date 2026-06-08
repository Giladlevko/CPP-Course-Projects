#include<iostream>
#include <iomanip>
#include<cmath>
#include<vector>
#include<algorithm>
using namespace std;


class point{
    public:
        point(long long a,long long b):x(a),y(b){}
        long long x;
        long long y;
};


bool sort_by_x(const point&a,const point&b){
    return a.x<b.x;
}

bool sort_by_y(const point&a,const point&b){
    return a.y<b.y;
}

long double distance(const point& a,const point& b){
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    return sqrt(dx*dx+dy*dy);
}


void merge_by_y(vector<point>& arr,int left,int mid,int right){
    int i = left;
    int j = mid+1;
    vector<point>temp;
    while(i<=mid&&j<=right){
        if(arr[i].y<arr[j].y){
            temp.push_back(arr[i]);
            i++;
        }
        else{
            temp.push_back(arr[j]);
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
}


long double closest_points(vector<point>&arr,int left,int right){
    if (right - left <= 2){
        long double min_d = 1e18;
        for(int i = left;i<=right;i++){
            for(int j = i+1;j<=right;j++){
                min_d = min(min_d,distance(arr[i],arr[j]));
            }
        }
        sort(arr.begin() + left, arr.begin() + right + 1, sort_by_y);
        return min_d;
    }

    int mid = left + (right - left) / 2;
    point mid_point = arr[mid];
    long double dist_l = closest_points(arr,left,mid);
    long double dist_r = closest_points(arr,mid+1,right);
    long double best_d = min(dist_l,dist_r);
    merge_by_y(arr,left,mid,right);
    vector<point> middle;
    //middle will be sorted by y already
    for(int i = left;i<=right;i++){
        if (abs(arr[i].x - mid_point.x)<best_d){
            middle.push_back(arr[i]);
        }
    }
    int middle_size = middle.size();
    sort(middle.begin(),middle.end(),sort_by_y);
    for(int i = 0;i<middle_size;i++){
        for(int j = i+1;j<middle_size&&(j-i)<=7;j++){
            best_d = min(best_d,distance(middle[i],middle[j]));
        }
    }
    return best_d;


}



int main(){
    cout << std::fixed << std::setprecision(4);
    int n = 0;
    vector<point>points;
    cin>>n;
    for(int i = 0;i<n;i++){
        int x = 0,y = 0;
        cin>>x>>y;
        point a = point(x,y);
        points.push_back(a);
    }
    sort(points.begin(),points.end(),sort_by_x);
    cout<<closest_points(points,0,n-1);
}
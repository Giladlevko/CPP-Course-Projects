//gets the minimum amount of points needed to connect ovelapping segments
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

class segment{
    public:
        segment(long long l,long long r):left(l),right(r){
            length = right - left;
            connected = false;
        }
        bool has(int point){
            if (point >= left && point <= right){
                return true;
            }
            return false;
        }
        long long left;
        long long right;
        long long length;
        bool connected;
};

bool sort_by_right(segment a,segment b){
    return a.right<b.right;
}

class segment_graph{
    public:
        segment_graph(int s):segment_count(s){
            overlapping_points_count = 0;
        }
        int segment_count;
        vector<segment> segments;

        int overlapping_points_count = 0;
        vector<long long>overlapping_points;


        vector<long long> best_overlapping_points(){
            if (segments.empty()) return overlapping_points;
            sort(segments.begin(),segments.end(),sort_by_right);
            long long current_point = segments[0].right;
            overlapping_points.push_back(current_point);
            overlapping_points_count++;
            for (int i = 1;i<segment_count;i++){
                if (!segments[i].has(current_point)){
                    current_point = segments[i].right;
                    overlapping_points.push_back(segments[i].right);
                    overlapping_points_count++;
                }
            }
            return overlapping_points;
        }

};

int main(){
    int seg_amount = 0;
    
    cin>>seg_amount;
    segment_graph graph = segment_graph(seg_amount);
    for (int i = 0; i< seg_amount;i++){
        long long l = 0;
        long long r = 0;
        cin >>l>>r;
        segment seg = segment(l,r);
        graph.segments.push_back(seg);
    }
    graph.best_overlapping_points();
    cout<<graph.overlapping_points_count<<endl;
    for (long long &point:graph.overlapping_points){
        cout << point<<" ";
    }
}
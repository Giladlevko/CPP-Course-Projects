//calculates the min amount of fuel stops for a given amount of gas stations and max distance
#include <iostream>
#include <vector>
using namespace std;

class vehicle{
    public:
        vehicle(int m = 0):max_miles(m){}
        int max_miles;
};

class road{
    public:
        road(int l= 0,int s_count = 0):length(l),stations_amount(s_count){}
        int length;
        int stations_amount;
        vector<int>gas_positions;

};

int best_stop(int &current_pos,road& path,vehicle& car){
    int best_gas = -1;
    for (int &pos:path.gas_positions){
        if (pos != -1 && pos <= current_pos+car.max_miles){
            if (pos > best_gas){
                best_gas = pos;
                pos = -1;
            }
        }
    }
    return best_gas;
}

int min_stops(road path,vehicle car){
    int current_pos = 0;
    int dist_left = path.length;
    int stop_count = 0;
    while(dist_left>car.max_miles){
        current_pos = best_stop(current_pos,path,car);
        if (current_pos == -1){
            stop_count = -1;
            break;
        }
        dist_left = path.length - current_pos;
        stop_count++;
    }
    return stop_count;
}

int main(){
    int max_miles = 0;
    int path_length = 0;
    int stops_amount = 0;
    cin>>path_length;
    cin>>max_miles;
    cin>>stops_amount;
    vehicle car = vehicle(max_miles);
    road path = road(path_length,stops_amount);
    for (int i = 0; i<stops_amount;i++){
        int s = 0;
        cin>>s;
        path.gas_positions.push_back(s);
    }
    cout<<min_stops(path,car);
}
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

class valuable{
    public:
        valuable(long long v,long long w):value(v),weight(w),taken(false){}
        long long value;
        long long weight;
        bool taken;
};

void mark_used_items(long long bag_val,vector<valuable>& items,const vector<vector<long long>>&values){
    int item_count = items.size()-1;
    int w_i_to_check = bag_val;
    for(int i = item_count;i>0;i--){
        if(values[w_i_to_check][i]!=values[w_i_to_check][i-1]){
            items[i].taken = true;
            w_i_to_check -= items[i].weight;
        }
    }
}

long long knapsack(const long long& max_w,const vector<valuable>& items){
    int item_count = items.size()-1;
    vector<vector<long long>> values(max_w+1,vector<long long>(item_count+1,0));
    for(int i = 1;i<=item_count;i++){
        for(int w = 1;w<=max_w;w++){
            values[w][i] = values[w][i-1];
            if (items[i].weight<=w){
                long long val = values[w-items[i].weight][i-1]+items[i].value;
                values[w][i] = max(val,values[w][i]);
            }
        }
    }
    return values[max_w][item_count];
}

int main(){
    long long sack_max = 0;
    long long item_count = 0;
    cin>>sack_max>>item_count;
    vector<valuable> items;
    items.push_back(valuable(0,0));
    for(int i = 0;i<item_count;i++){
        long long a = 0;
        cin>>a;
        valuable item(a,a);
        items.push_back(item);
    }
    cout<<knapsack(sack_max,items);
}
#include<iostream>
#include<vector>
#include<numeric>
#include<algorithm>
using namespace std;

class valuable{
    public:
        valuable(long long v,long long w):value(v),weight(w),taken(false){}
        long long value;
        long long weight;
        bool taken;
};


int can_split(vector<valuable>& items){
    long long total_value = accumulate(items.begin(),items.end(),0LL,
    [](long long current_sum, const valuable& b){return (current_sum + b.value);});
    long long val_for_each;
    if (total_value%3 == 0){
        val_for_each = total_value/3;
    }
    else{return 0;}

    int item_count = items.size()-1;
    
    vector<vector<vector<bool>>> values(val_for_each+1,vector<vector<bool>>(val_for_each+1,vector<bool>(item_count+1,false)));
    values[0][0][0] = true;
    //knapsack loops
    for(int i = 1;i<=item_count;i++){
        long long w_i = items[i].weight;
        for(int w1 = 0;w1<=val_for_each;w1++){
            for(int w2 = 0; w2<= val_for_each;w2++){
                if(values[w1][w2][i-1]){values[w1][w2][i] = true;}
                if(w1 >= w_i && values[w1-w_i][w2][i-1]){values[w1][w2][i] = true;}
                if(w2 >= w_i && values[w1][w2-w_i][i-1]){values[w1][w2][i] = true;}
            }
        }
    }
    return values[val_for_each][val_for_each][item_count] ? 1:0;
}

int main(){
    long long item_count = 0;
    cin>>item_count;
    vector<valuable> items;
    items.push_back(valuable(0,0));
    for(int i = 0;i<item_count;i++){
        long long a = 0;
        cin>>a;
        valuable item(a,a);
        items.push_back(item);
    }
    cout<<can_split(items);
}
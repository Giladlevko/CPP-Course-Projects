#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
using namespace std;

class valuable{
    public:
        valuable(long double v = 0.0, long double w = 0.0):value(v),weight(w){}
        long double value;
        long double weight;
};

class bag{
    public:
        bag(long double v = 0.0,long double w = 0.0):
        val(v),weight(w){}

        long double val;
        long double weight;
        vector<valuable> items;
};

void gen_items(vector<valuable>& items,int size){
    random_device rd;
    mt19937 gen(rd());
    for(int i = 0;i<size;i++){
        // value and weight from 0 to 10
        long double v = (static_cast<double>(gen()%100))/10.0;
        long double w = (static_cast<double>(gen()%100))/10.0;
        valuable item = valuable(v,w);
        items.push_back(item);
        cout<<"("<<item.value<<","<<item.weight<<"), ";
        
    }
    cout<<endl;
}

valuable* best_item(vector<valuable>& items){
    long double max_value_per_weight = 0;
    valuable* best_item = nullptr;
    for(valuable &v:items){
        if (v.weight > 0 && v.value/v.weight > max_value_per_weight){
            max_value_per_weight = v.value/v.weight;
            best_item = &v;
        }
    }
    return best_item;
}


bag knapsack(vector<valuable>& items,long double weight_left){
    bag sack = bag(0.0,weight_left);
    while(weight_left > 0){
        valuable* item = best_item(items);
        if (item == nullptr){break;}
        long double a = (item->weight <= weight_left)?item->weight:weight_left;
        sack.val += a *(item->value/item->weight);
        item->weight-=a;
        weight_left -= a;
    }
    return sack;
}

int main(){
    cout << fixed << setprecision(4);
    vector<valuable> items;
    //for inputs
    int n = 0,w = 0;
    cin>>n>>w;
    for(int i = 0; i<n;i++){
        int c_i= 0,w_i = 0;
        cin>>c_i>>w_i;
        valuable item = valuable(c_i,w_i);
        items.push_back(item);
    }

    
    //if you want randomly generated items:
    // gen_items(items,10)
    
    bag sack = knapsack(items,w);
    cout<<sack.val<<endl;
}
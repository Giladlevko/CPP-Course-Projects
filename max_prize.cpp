//recieves a number, splits that number into boxes such that the it is the maximum amount of boxes
// that the number can split into as a different number each time
#include<iostream>
#include<vector>
using namespace std;

class box{
    public:
        box(int v):val(v){}
        long long val;
};

class box_arr{
    public:
        box_arr(long long v):num_to_split(v){box_count = 0;}
        long long num_to_split;
        int box_count;
        vector<box> boxes;
        void split_num(){
            long long amount_left = num_to_split;
            long long prev_num = 0; 
            while(amount_left>0){
                long long cur_val = prev_num+1;
                long long next_val = cur_val+1;
                if ((amount_left -(cur_val + next_val))<0){cur_val = (amount_left);}
                box b = box(cur_val);
                prev_num = cur_val;
                boxes.push_back(b);
                box_count++;
                amount_left -= cur_val;
            }
        }
        void print_boxes(){
            for (box &b:boxes){
                cout<<b.val<<" ";
            }
        }

};

int main(){
    long long n = 0;
    cin>>n;
    box_arr boxes = box_arr(n);
    boxes.split_num();
    cout<<boxes.box_count<<endl;
    boxes.print_boxes();
    return 0;
}
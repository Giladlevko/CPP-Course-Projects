#include<vector>
#include<iostream>
#include<climits>
using namespace std;

struct node{
    node(long long k,int l,int r):key(k),left_indx(l),right_indx(r){}
    long long key;
    int left_indx;
    int right_indx;
};

class tree{
    public:
        tree(int r = 0):root_indx(r){}
        int root_indx;
        vector<node> nodes;
        vector<long long> traversed_keys;
        bool in_order_is_bst(int node_indx,long long max,long long min){

            if (node_indx == -1 || nodes.empty()){return true;}

            long long current_key = nodes[node_indx].key;
            if (current_key <min || current_key> max){
                return false;
            }

            bool t1 = in_order_is_bst(nodes[node_indx].left_indx,current_key-1,min);

            bool t2 = in_order_is_bst(nodes[node_indx].right_indx,max,current_key);

            return (t1 && t2);
        }
        
};

int main(){
    int tree_size = 0;
    tree t(0);
    cin>>tree_size;
    for(int i = 0; i<tree_size;i++){
        int k,l,r;
        cin>>k>>l>>r;
        node n(k,l,r);
        t.nodes.push_back(n);
    }
    if(t.in_order_is_bst(t.root_indx,LLONG_MAX,LLONG_MIN)){
        cout<<"CORRECT";
    }
    else{
        cout<<"INCORRECT";
    }
    return 0;
}
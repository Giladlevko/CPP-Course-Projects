#include<vector>
#include<iostream>
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
        void in_order(int node_indx){
            if (node_indx == -1 || nodes.empty()){return;}
            in_order(nodes[node_indx].left_indx);
            traversed_keys.push_back(nodes[node_indx].key);
            in_order(nodes[node_indx].right_indx);
        }
        bool is_bst(){
            if (nodes.empty()){return true;}
            in_order(root_indx);
            int size = traversed_keys.size();
            for (int i = 1 ;i<size;i++){
                if (traversed_keys[i]<traversed_keys[i-1]){
                    return false;
                }
            }
            return true;
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
    if(t.is_bst()){
        cout<<"CORRECT";
    }
    else{
        cout<<"INCORRECT";
    }
    return 0;
}
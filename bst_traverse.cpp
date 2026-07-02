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

        void pre_order(int node_indx){
            if (node_indx == -1 || nodes.empty()){return;}
            cout<<nodes[node_indx].key<<" ";
            pre_order(nodes[node_indx].left_indx);
            pre_order(nodes[node_indx].right_indx);
        }
        void post_order(int node_indx){
            if (node_indx == -1 || nodes.empty()){return;}
            post_order(nodes[node_indx].left_indx);
            post_order(nodes[node_indx].right_indx);
            cout<<nodes[node_indx].key<<" ";
        }
        void in_order(int node_indx){
            if (node_indx == -1 || nodes.empty()){return;}
            in_order(nodes[node_indx].left_indx);
            cout<<nodes[node_indx].key<<" ";
            in_order(nodes[node_indx].right_indx);
            
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
    t.in_order(t.root_indx);
    cout<<"\n";
    t.pre_order(t.root_indx);
    cout<<"\n";
    t.post_order(t.root_indx);
    cout<<"\n";
}
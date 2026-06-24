#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
using namespace std;

class node{
    public:
        node(int v = 0):val(v){}
        int val;
        vector<node*>children;
};


int bfs_depth(node*tree){
    int depth = 0;
    if (tree == nullptr){return 0;}
    queue<node*> q;
    q.push(tree);
    while(!q.empty()){
        depth++;
        int level_width = q.size();
        for(int i=0;i<level_width;i++){
            node* current= q.front();
            q.pop();
            for (node*child:current->children){
                q.push(child);
            }
        }
    }
    return depth;
}

int main(){
    int n = 0;
    cin>>n;
    vector<node> nodes(n);
    node* root;
    for(int i = 0;i<n;i++){
        nodes[i].val = i;
    }
    for(int i = 0;i<n;i++){
        int parent_indx = 0;
        cin >> parent_indx;
        if (parent_indx == -1){
            root = &nodes[i];
        }
        else{
            nodes[parent_indx].children.push_back(&nodes[i]);
        }
    }
    cout<<bfs_depth(root);
    return 0;
}
#include<vector>
#include<string>
#include<iostream>
#include<unordered_map>

using namespace std;

//takes string, its suffix array, and its lcp array and creates a suffix tree

struct node{
    node(node*pa,int dep,int s,int e):
    parent(pa),depth(dep),start(s),end(e),visited(false){}
    node*parent;
    unordered_map<char,node*> children;
    int depth;
    int start;
    int end;
    bool visited;
};

node* create_new_node(node*parent,const string&s,const int& suffix){
    int size = s.size();
    int depth = size - suffix;
    int start = suffix + parent->depth;
    int end = size - 1;
    node* leaf = new node(parent,depth,start,end);
    parent->children[s[leaf->start]] = leaf;
    return leaf;
}

node* break_edge(node*original,const string&s,const int& start,const int& offset){
    char start_char = s[start];
    char mid_char = s[start+offset];
    int depth = original->depth +offset;
    int end = start + offset -1;
    node* mid_node = new node(original,depth,start,end);
    mid_node->children[mid_char] = original->children[start_char];
    original->children[start_char]->parent = mid_node;
    original->children[start_char]->start += offset;
    original->children[start_char] = mid_node;
    return mid_node;
}

class suffix_tree{
    public:
        suffix_tree(string s, vector<int>suffix_arr, vector<int>lcp_arr){
            full_string = s;
            root = new node(nullptr,0,-1,-1);
            root->visited = true;
            int lcp_prev = 0;
            int size = s.size();
            node* cur_node = root;
            for(int i = 0; i<size; i++){
                int suffix = suffix_arr[i];
                while(cur_node->depth>lcp_prev){
                    cur_node = cur_node->parent;
                }
                if(cur_node->depth == lcp_prev){
                    cur_node = create_new_node(cur_node,s,suffix);
                }
                else{
                    int start = suffix_arr[i-1] + cur_node->depth;
                    int offset = lcp_prev - cur_node->depth;
                    node* mid_node = break_edge(cur_node,s,start,offset);
                    cur_node = create_new_node(mid_node,s,suffix);
                }
                leaves.push_back(cur_node);
                if(i<size-1){
                    lcp_prev = lcp_arr[i];
                }
            }
        }
        void print(){
            cout<<full_string<<"\n";
            for(node*leaf:leaves){
                node*cur = leaf;
                vector<node*>path;
                while(!cur->visited){
                    path.push_back(cur);
                    cur->visited = true;
                    cur = cur->parent;
                }
                for(int i = path.size()-1; i>= 0; i--){
                    node* n = path[i];
                    cout<<n->start<<" "<<n->end+1<<"\n";
                }
            }
        }
    private:
        node*root;
        string full_string;
        vector<node*>leaves;
};


int main(){
    string s;
    cin>>s;
    int size = s.size();

    vector<int>suffix_arr(size),lcp_arr(size-1);
    
    for(int i = 0; i<size; i++){
        cin>>suffix_arr[i];
    }
    for(int i = 0; i<size-1; i++){
        cin>>lcp_arr[i];
    }
    suffix_tree st(s,suffix_arr,lcp_arr);
    st.print();
}
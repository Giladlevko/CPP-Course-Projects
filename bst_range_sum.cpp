#include<vector>
#include<iostream>
#include<climits>
using namespace std;

struct node{
    node(long long k,node* p = nullptr):key(k),left(nullptr),right(nullptr),parent(p),st_sum(k){}
    long long key;
    long long st_sum;
    node* parent;
    node* left;
    node* right;
};

class tree{
    public:
        tree(node* r = nullptr):root(r){}
        node* root;
        vector<node> nodes;
        vector<long long> traversed_keys;

        node* find(long long k,node* r){
            if (r == nullptr){return r;}
            if (r->key == k){
                return  r;
            }
            if (r->key>k){
                if (r->left != nullptr){
                    return (find(k,r->left));
                }
                return  r;
            }
            else{
                if(r->right != nullptr){
                    return (find(k,r->right));
                }
                return  r;
            }

        }

        node* search(long long k){
            node* p = find(k,root);
            if(p!=nullptr){
                splay(p);
            }
            return p;
        }

        void add(long long k){
            node* p = search(k);
            if (p != nullptr && p->key == k){
                return;
            }
            node* c = new node(k,nullptr);
            if (p == nullptr){
                root = c;
                return;
            }
            else if (k < p->key){
                c->right = p;
                c->left = p->left;
                if(p->left!=nullptr){p->left->parent = c;}
                p->left = nullptr;
            }
            else{
                c->left = p;
                c->right = p->right;
                if(p->right!=nullptr){p->right->parent = c;}
                p->right = nullptr;
            }
            p->parent = c; 
            root = c;
           update_tree_sums(p);
        }



        node* left_desendant(node* n){
            if(n->left == nullptr){
                return n;
            }
            else{
                return left_desendant(n->left);
            }
        }

        node* right_ancestor(node* n){
            if (n == nullptr || n->parent == nullptr ){return nullptr;}
            if (n == n->parent->left){
                return(n->parent);
            }
            else{
                return(right_ancestor(n->parent));
            }
        }

        node* next(node* n){
            if (n == nullptr){return nullptr;}
            if(n->right != nullptr){
                return left_desendant(n->right);
            }
            else{
                return right_ancestor(n);
            }
        }

        void update_tree_sums(node* n){
            node* cur = n;
            while (cur != nullptr) {
                update_sum(cur);
                cur = cur->parent;
            }
        }

        void remove(node* n){
            if (n == nullptr){return;}
            node* p = search(n->key);
            if(p->key != n->key || p != root){return;}
            
            node* left_tree = root->left;
            node* right_tree = root->right;
            if(left_tree!=nullptr){left_tree->parent = nullptr;}
            if(right_tree!=nullptr){right_tree->parent = nullptr;}

            delete root;
            if(left_tree == nullptr){root = right_tree;}
            else{
                root = left_tree;
                search(LLONG_MAX);
                root->right = right_tree;
                if (right_tree!=nullptr){right_tree->parent = root;}
            }
            update_sum(root);
        }



        void remove_by_key(long long k){
            node* n = search(k);
            if (n != nullptr && n->key == k){
                remove(n);
            }
        }




        void update_sum(node* n){
            if(n == nullptr){return;}
            n->st_sum = n->key;
            if (n->left != nullptr){
                n->st_sum += n->left->st_sum;
            }
            if (n->right != nullptr){
                n->st_sum += n->right->st_sum;
            }
        }
        long long prefix_sum(long long target){
            if (root == nullptr) {return 0;}
            node*n = search(target);
            long long left_sum = (n->left != nullptr) ? n->left->st_sum : 0;
            if (n->key <= target){
                return n->key + left_sum;
            }
            else{
                return left_sum;
            }
        }

        long long range_sum(long long x,long long y){
            if (x>y){return 0;}
            return prefix_sum(y) - prefix_sum(x - 1);
        }

        void rotate_right(node* x){
            node* p = x->parent;
            node* y = x->left;
            if(y==nullptr){return;}
            node* B = y->right;
            y->parent = p;
            if (p != nullptr){
                if (x == p->left){p->left = y;}
                else{p->right = y;}
            }
            else{
                root = y;
            }
            x->parent = y;
            y->right = x;
            x->left = B;
            if(B!=nullptr){B->parent = x;}
            update_sum(x);
            update_sum(y);
        }

        void rotate_left(node* x){
            node* p = x->parent;
            node* y = x->right;
            if(y==nullptr){return;}
            node* B = y->left;
            y->parent = p;
            if (p != nullptr){
                if (x == p->left){p->left = y;}
                else{p->right = y;}
            }
            else{
                root = y;
            }
            x->parent = y;
            y->left = x;
            x->right = B;
            if(B!=nullptr){B->parent = x;}
            update_sum(x);
            update_sum(y);
        }

        void splay(node* x){
            if(x==nullptr){return;}
            while(x->parent!=nullptr){
                node* p = x->parent;
                node* g = p->parent;
                if(g==nullptr){//zig case
                    if(x==p->left){rotate_right(p);}
                    else{rotate_left(p);}
                }
                else if(x==p->left && p==g->left){//zig zig case, both are left children
                    rotate_right(g);
                    rotate_right(p);
                }
                else if(x==p->right && p==g->right){//zig zig, both are right
                    rotate_left(g);
                    rotate_left(p);
                }
                else if(x==p->right&& p==g->left){//zig zag right left
                    rotate_left(p);
                    rotate_right(g);
                }
                else{//zig zag left right
                    rotate_right(p);
                    rotate_left(g);
                }
            }
        }

        
};

int main(){
    long long M = 1000000001;
    long long x = 0;
    int ops = 0;
    tree t = tree(nullptr);
    cin>>ops;
    for(int i = 0; i<ops;i++){
        char action = ' ';
        long long l = 0;
        cin>>action>>l;
        if(action == '+'){
            t.add((l+x)%M);
        }
        else if(action == '-'){
            t.remove_by_key((l+x)%M);
        }
        else if(action == '?'){
            node* n = t.search((l+x)%M);
            if( n != nullptr && n->key == (l+x)%M){
                cout<<"Found\n";
            }
            else{
                cout<<"Not found\n";
            }
        }
        else if(action == 's'){
            long long r = 0;
            cin>>r;
            x = t.range_sum((l+x)%M,(r+x)%M);
            cout<<x<<"\n";
        }
    }
    return 0;
}
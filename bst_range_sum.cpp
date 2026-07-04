#include<vector>
#include<iostream>
using namespace std;

struct node{
    node(long long k,node* p = nullptr):key(k),left(nullptr),right(nullptr),parent(p)st_sum(0){}
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
                return r;
            }
            if (r->key>k){
                if (r->left != nullptr){
                    return(find(k,r->left));
                }
                return r;
            }
            else{
                if(r->right != nullptr){
                    return (find(k,r->right));
                }
                return r;
            }
        }

        void add(long long k){
            node* p = find(k,root);
            if (p != nullptr && p->key == k){return;}
            node* c = new node(k,p);
            if (p == nullptr){
                root = c;
            }
            else if (k < p->key){
                p->left = c;
            }
            else{
                p->right = c;
            }
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

        void remove(node* n){
            node* p = n->parent;
            node* l = n->left;
            if (n->right == nullptr){
                if (p == nullptr){
                    root = l;
                }
                else if (n == p->left){
                    p->left = l;
                }
                else{
                    p->right = l;
                }
                if (l != nullptr){
                    l->parent = p;
                }
                delete n;
            }
            else{
                node* x = next(n);
                //x has no left child
                if(x == n->right){
                    x->left = n->left;
                    if (n->left != nullptr){n->left->parent = x;}
                    x->parent = p;
                    if (p != nullptr){
                        if (n == p->left){
                            p->left = x;
                        }
                        else{
                            p->right = x;
                        }
                    }
                    else{root = x;}
                }
                else{
                    if(x->right != nullptr){
                        x->right->parent = x->parent;
                    }
                    x->parent->left = x->right;

                    if (n->left != nullptr){n->left->parent = x;}
                    if (n->right != nullptr){n->right->parent = x;}
                    x->left = n->left;
                    x->right = n->right;
                    x->parent = p;

                    if (p == nullptr){
                        root = x;
                    }
                    else{
                        if (n == p->left){
                            p->left = x;
                        }
                        else{p->right = x;}
                    } 
                }
                delete n;
            }
        }

        void remove_by_key(long long k){
            node* n = find(k,root);
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
        long long prefix_sum(node* n,long long target){
            if (n == nullptr) {return 0}
            if (n->key <= target){
                long long left_sum = (n->left != nullptr) ? n->left->st_sum : 0;
                return n->key + left_sum + prefix_sum(n->right, target);
            }
            else{
                return prefix_sum(n->left,target);
            }
        }

        long long range_sum(long long x,long long y){
            if (x>y){return 0;}
            return prefix_sum(root, y) - prefix_sum(root, x - 1);
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
            node* n = t.find((l+x)%M,t.root);
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
}
#include<vector>
#include<iostream>
#include<climits>
#include<utility>
#include<string>
using namespace std;

struct node{
    node(char c,long long s = 1,node* p = nullptr):character(c),size(s),left(nullptr),right(nullptr),parent(p){}
    long long size;
    char character;
    node* parent;
    node* left;
    node* right;
};

class tree{
    public:
        tree(node* r = nullptr):root(r){}
        node* root;

        node* find(long long k,node* r){
            if (r == nullptr){return r;}
            long long left_size = (r->left != nullptr) ? r->left->size : 0;
            if (left_size == k){
                return  r;
            }
            if (left_size > k){
                return (find(k,r->left));
            }
            else{
                if(r->right != nullptr){
                    return (find(k-left_size-1,r->right));
                }
                return  r;
            }

        }

        node* search(long long k,node* r = nullptr){
            if(r==nullptr){r = root;}
            node* p = find(k,r);
            if(p!=nullptr){
                splay(p);
            }
            if(r==root){root = p;}
            return p;
        }

        void update_tree_sizes(node* n){
            node* cur = n;
            while (cur != nullptr) {
                update_size(cur);
                cur = cur->parent;
            }
        }


        void update_size(node* n){
            if(n == nullptr){return;}
            n->size = 1;
            if (n->left != nullptr){
                n->size += n->left->size;
            }
            if (n->right != nullptr){
                n->size += n->right->size;
            }
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
           // else{
           //     root = y;
            //}
            x->parent = y;
            y->right = x;
            x->left = B;
            if(B!=nullptr){B->parent = x;}
            update_size(x);
            update_size(y);
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
           // else{
           //     root = y;
            //}
            x->parent = y;
            y->left = x;
            x->right = B;
            if(B!=nullptr){B->parent = x;}
            update_size(x);
            update_size(y);
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

        
        node* merge(node*r1,node*r2){
            if(r1==nullptr){return r2;}
            if(r2==nullptr){return r1;}
            node* t = search(r1->size-1,r1);
            t->right = r2;
            r2->parent = t;
            update_size(t);
            return t;
        }

        pair<node*,node*> split(long long x,node* r){
            if (r == nullptr){
                return{nullptr,nullptr};
            }
            if(x==0){return{nullptr,r};}
            if(x >= r->size){ return{r,nullptr};}

            r = search(x,r);
            node*left_tree = r->left;
            if(left_tree != nullptr){left_tree->parent = nullptr;}
            r->left = nullptr;
            update_size(r);
            return{left_tree,r};

        }

        
};

tree* build_tree_from_string(string& s){
    node* root = new node(s[0]);
    tree* t = new tree(root);
    long long size = s.size();
    for(int i = 1;i<size;i++){
        node* new_node = new node(s[i]);
        t->root = t->merge(t->root,new_node);
    }
    return t;
}

void change_string_layout(int i,int j,int k,tree* t){
    pair<node*,node*> split_1 = t->split(i,t->root);//before i + i to end
    pair<node*,node*> split_2 = t->split(j-i+1,split_1.second);//i to j + after j
    node* rem = t->merge(split_1.first,split_2.second);//merge the before i and after j parts
    pair<node*,node*> split_3 = t->split(k,rem);//splits the remainig elements at k
    t->root = t->merge(t->merge(split_3.first,split_2.first),split_3.second);
}

void print_tree(node* r){
    if(r == nullptr){return;}
    print_tree(r->left);
    cout<<r->character;
    print_tree(r->right);
}

int main(){
    string s;
    cin>>s;
    tree* t = build_tree_from_string(s);
    int i,j,k,n;
    cin>>n;
    for(int l = 0; l<n;l++){
        cin>>i>>j>>k;
        change_string_layout(i,j,k,t);
    }
    print_tree(t->root);

    return 0;

}
#include<string>
#include<vector>
#include<iostream>
#include<unordered_map>
using namespace std;


struct node{
    node(char c,int n,int pn):letter(c),number(n),parent_num(pn){}
    char letter;
    int number;
    int parent_num;
    unordered_map<char,node*> nexts;
};

class trie{
    public:
        trie(const vector<string>patterns){
            int count = 0;
            root = new node('*',count,0);
            for(const string& p:patterns){
                node* current_node = root;
                for(int i = 0; i< p.size();i++){
                    char curr_symb = p[i];
                    auto it = current_node->nexts.find(curr_symb);
                    if(it != current_node->nexts.end()){
                        current_node = it->second;
                    }
                    else{
                        node* new_node = new node(curr_symb,++count,current_node->number);
                        nodes.push_back(new_node);
                        current_node->nexts[curr_symb] = new_node;
                        current_node = new_node;
                    }
                }
            }
        }

        void print(){
            for(node* n:nodes){
                cout<<n->parent_num<<"->"<<n->number<<":"<<n->letter<<"\n";
            }
        }

        node*root;
        vector<node*>nodes;
};



int main(){
    int p_count;
    cin>>p_count;
    vector<string> patterns;
    for(int i = 0; i<p_count;i++){
        string p;
        cin>>p;
        patterns.push_back(p);
    }
    trie t(patterns);
    t.print();
}
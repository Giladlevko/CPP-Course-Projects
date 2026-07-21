#include<string>
#include<vector>
#include<iostream>
#include<unordered_map>
using namespace std;


struct node{
    node(char c,int n,int pn,bool end):letter(c),number(n),parent_num(pn),pat_end(end){}
    char letter;
    int number;
    int parent_num;
    bool pat_end;
    unordered_map<char,node*> nexts;
};

class trie{
    public:
        trie(const vector<string>patterns){
            int count = 0;
            root = new node('*',count,0,false);
            for(const string& p:patterns){
                node* current_node = root;
                for(int i = 0; i< p.size();i++){
                    char curr_symb = p[i];
                    auto it = current_node->nexts.find(curr_symb);
                    if(it != current_node->nexts.end()){
                        current_node = it->second;
                        if(i == p.size()-1){current_node->pat_end = true;}
                    }
                    else{
                        node* new_node = new node(curr_symb,++count,current_node->number,i == p.size()-1);
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

        void matching(string& txt){
            int pos = 0;
            vector<int>matching_positions;
            while(!txt.empty()){
                int match_pos = prefix_match(txt,pos);
                txt.erase(0,1);
                pos++;
                if(match_pos!=-1){
                    matching_positions.push_back(match_pos);
                }
            }
            for(const int& i:matching_positions){
                cout<<i<<" ";
            }
        }

        int prefix_match(const string& txt,int pos = 0){
            int next_letter = 0;
            char symb = txt[next_letter];
            node* v = root;
            while(true){
                if(v->nexts.empty() || v->pat_end){
                    return pos;
                }
                else{
                    auto it = v->nexts.find(symb);
                    if(it != v->nexts.end()){
                        symb = txt[++next_letter];
                        v = it->second;
                        
                    }
                    else{
                        return -1;
                    }
                }
            }
        }

        node*root;
        vector<node*>nodes;
};



int main(){
    string txt;
    cin>>txt;
    int p_count;
    cin>>p_count;
    vector<string> patterns;
    for(int i = 0; i<p_count;i++){
        string p;
        cin>>p;
        patterns.push_back(p);
    }
    trie t(patterns);
    t.matching(txt);
}
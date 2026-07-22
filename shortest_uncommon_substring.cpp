#include<string>
#include<vector>
#include<iostream>
#include<unordered_map>
#include<queue>
#include<utility>
using namespace std;

enum class substring_group {first,second,both};

struct node{
    node(int s,int e,bool is_end):start(s),end(e),pat_end(is_end){}
    int start,end;
    bool pat_end;
    substring_group group;
    unordered_map<char,node*> nexts;
};

class compressed_suffix_trie{
    public:
        compressed_suffix_trie(const vector<int>patterns,const string&text){
            full_text = text;
            int count = 0;
            root = new node(-1,-1,false);
            int last_letter = text.size()-1;
            for(int p:patterns){
                int i = p;
                char p_first_letter = text[p];
                if (p_first_letter == '#'){first_text_end = i;} 
                node* current_node = root;
                while(i<=last_letter){
                    char curr_symb = text[i];
                    
                    auto it = current_node->nexts.find(curr_symb);
                    if(it != current_node->nexts.end()){
                        node* child = it->second;
                        int k = child->start;
                        while(k <= child->end && i <= last_letter && text[k] == text[i]){
                            k++;i++;
                        }
                        if(k > child->end){
                            //matched the entire edge so I can continue with the while loop
                            current_node = child;
                        }
                        else{//needs to split an edge and uses k to know where the mismatch happend

                            //the node where there needs to be 2 children
                            node* split_node = new node(child->start,k-1,false);
                            current_node->nexts[curr_symb] = split_node;//replace the child with the split node
                            //make the child start at the split point. 
                            //The end remains the same as before
                            child->start = k;
                            split_node->nexts[text[k]] = child;//make the child a child of the split node
                            if(i<=last_letter){//making sure i is in bounds
                                //sets the start to where i was when there was a mismatch, 
                                // and the end to the last letter
                                node* leaf_node = new node(i,last_letter,true);
                                split_node->nexts[text[i]] = leaf_node;
                            }
                            break;
                        }
                    }
                    else{
                        node* new_node = new node(i,last_letter,true);
                        current_node->nexts[curr_symb] = new_node;
                        break;
                    }
                }
            }
            assign_groups();
        }

        void group_node(node* curr){
            for(auto&p:curr->nexts){
                group_node(p.second);
            }
            substring_group curr_group = (curr->start <= first_text_end) ? substring_group::first : substring_group::second;
            curr->group = curr_group;
            for(auto&p:curr->nexts){
                node* child = p.second;
                if(child->group!=curr_group){
                    curr->group = substring_group:: both;
                    break;
                }
            }
        }

        void assign_groups(){
            for(auto&p:root->nexts){
                group_node(p.second);
            }
        }

        pair<int,int> find_shortest_uncommon_substring(){
            queue<node*>q;
            q.push(root);
            while(!q.empty()){
                int size = q.size();
                for(int i = 0; i<size ;i++){
                    node* curr = q.front();
                    q.pop();
                    if(curr != root){
                        int start = curr->start;
                        char first_letter = full_text[start];
                        if(curr->group==substring_group::first && first_letter != '#' && first_letter != '$'){
                            int end = curr->end;
                            return{start,end};
                        }
                    }
                    
                    for(auto&p: curr->nexts){
                        q.push(p.second);
                    }
                }
            }
            return{-1,-1};
        }

        void print(node* curr){
           for(auto& p:curr->nexts){
            node* child = p.second;
            int length = child->end - child->start + 1;
            cout.write(&full_text[child->start],length)<<"\n";
            print(child);
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
            node* v = root;
            while(true){
                if(v->nexts.empty() || v->pat_end){
                    return pos;
                }
                else{
                    auto it = v->nexts.find(txt[next_letter]);
                    if(it != v->nexts.end()){
                        ++next_letter;
                        v = it->second;
                        
                    }
                    else{
                        return -1;
                    }
                }
            }
        }
        int first_text_end;
        string full_text;
        node*root;
};

class suffix{
    public:
        suffix(const string& text){
            full_text = text;
            vector<int>patterns;
            for(int i = 0; i<text.size();i++){
                patterns.push_back(i);
            }
            trie_tree = new compressed_suffix_trie(patterns,text);
        }

        void print(){
            if(trie_tree!=nullptr){
                trie_tree->print(trie_tree->root);
            }
        }
        void shortest_non_shared_substring(){
            pair<int,int>p = trie_tree->find_shortest_uncommon_substring();
            if(p.first == -1){cout<<"ERROR\n";return;}
            int length = p.second - p.first + 1;
            char last_letter = full_text[p.second];
            cout.write(&full_text[p.first],length)<<"\n";
        }
    private:
        string full_text;
        compressed_suffix_trie* trie_tree = nullptr;
};



int main(){
    string txt1,txt2;
    cin>>txt1>>txt2;
    txt1.append("#");
    txt2.append("$");
    txt1.append(txt2);
    suffix s(txt1);
    //s.print();
    s.shortest_non_shared_substring();
}
#include<string>
#include<vector>
#include<iostream>
#include<unordered_map>
#include<queue>
#include<utility>
using namespace std;

enum class substring_group {first = 0,second = 1,both = 2,nothing = 3};

struct node{
    node(int s,int e,int suf_s,bool is_end):
    start(s),end(e),suffix_start(suf_s),pat_end(is_end),group(substring_group::nothing){}
    int start,end,suffix_start;
    bool pat_end;
    substring_group group;
    unordered_map<char,node*> nexts;
};

class compressed_suffix_trie{
    public:
        compressed_suffix_trie(const vector<int>patterns,const string&text){
            full_text = text;
            int count = 0;
            root = new node(-1,-1,-1,false);
            int last_letter = text.size()-1;
            for(int p:patterns){
                int i = p;
                char p_first_letter = text[p];
                if (p_first_letter == '#'){first_text_end = i-1;} 
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

                            //the node where there needs to be 2 children, 
                            // split node does not have a suffix start so set to -1
                            node* split_node = new node(child->start,k-1,p,false);
                            current_node->nexts[curr_symb] = split_node;//replace the child with the split node
                            //make the child start at the split point. 
                            //The end remains the same as before
                            child->start = k;
                            split_node->nexts[text[k]] = child;//make the child a child of the split node
                            if(i<=last_letter){//making sure i is in bounds
                                //sets the start to where i was when there was a mismatch, 
                                // and the end to the last letter
                                node* leaf_node = new node(i,last_letter,p,true);
                                split_node->nexts[text[i]] = leaf_node;
                            }
                            break;
                        }
                    }
                    else{
                        node* new_node = new node(i,last_letter,p,true);
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
            if(curr->nexts.empty()){
                curr->group = (curr->suffix_start <= first_text_end) ? substring_group::first : substring_group::second;
                return;
            }
            substring_group curr_group = substring_group::nothing;
            for(auto&p:curr->nexts){
                node* child = p.second;
                if(curr_group==substring_group::nothing){
                    curr_group = child->group;
                }
                else if (child->group!=curr_group){
                    curr_group = substring_group::both;
                    break;
                }
            }
            curr->group = curr_group;
        }

        void assign_groups(){
            for(auto&p:root->nexts){
                group_node(p.second);
            }
        }

        void find_shortest_non_shared_substring_dfs(
            node* curr,int depth,
            int& min_len,
            pair<int,int>&best
        ){
            for(auto& p:curr->nexts){
                node* child = p.second;

                if(child->group == substring_group::first){
                    //of shared path, add 1 letter to the previous shared to get the min non shared
                    //cant just take the substring because txt2 might have it but the combination 
                    //of the previous shared then one from txt1 will be a non shared substring
                    int start_index = child->suffix_start;
                    int substring_length = depth + 1;
                    if(start_index + substring_length - 1 <= first_text_end){
                        if(substring_length<min_len){
                            min_len = substring_length;
                            //updates the best substring coordinates
                            best = {start_index,start_index+substring_length-1};
                        }//no recursive call here because it will only result in a longer substring
                    }

                }
                else{
                    if(child->group == substring_group::both){
                        //if I am still on a shared path then I add the whole shared sunstring length
                        int edge_length = child->end - child->start + 1;
                        find_shortest_non_shared_substring_dfs(child,depth+edge_length,min_len,best);
                    }
                }
            }
        }


        pair<int,int> find_shortest_uncommon_substring(){
            pair<int,int>best = {-1,-1};
            int min_len = 1e9;
            int depth = 0,start_index = -1;
            find_shortest_non_shared_substring_dfs(root,depth,min_len,best);
            return best;
        }

        void print(node* curr){
           for(auto& p:curr->nexts){
            node* child = p.second;
            int length = child->end - child->start + 1;
            cout.write(&full_text[child->start],length)<<"\n";
            print(child);
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
    s.shortest_non_shared_substring();
}
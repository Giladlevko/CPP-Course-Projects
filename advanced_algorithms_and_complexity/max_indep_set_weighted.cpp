#include<vector>
#include<iostream>

using namespace std;

const int INF = 1e15;

struct node{
    node(vector<node*>c = {},long long w = 0,long long sw = INF):
    children(c),weight(w),set_weight(sw){}
    vector<node*>children;
    long long weight;
    long long set_weight;
};


//we want to maximize fun and make sure there are no direct relations between the people
//because it would mean a boss and their worker would be in the party and thats not fun
long long fun_party(node* v){
    if(v->set_weight == INF){
        if(v->children.empty()){
            v->set_weight = v->weight;
        }
        else{
            long long m_1 = v->weight;
            for(node* child:v->children){
                for(node* gran_child:child->children){
                    m_1 += fun_party(gran_child);
                }
            }
            long long m_0 = 0;
            for(node* child:v->children){
                m_0 += fun_party(child);
            }
            v->set_weight = (m_1>=m_0) ? m_1 : m_0;
        }
    }
    return v->set_weight;
}


void build_tree(
    const int& curr,
    const int& parent,
    const vector<vector<int>>&adj_list,
    vector<node*>&people_arr
){
    for(int neighbor:adj_list[curr]){
        if(neighbor != parent){
            people_arr[curr]->children.push_back(people_arr[neighbor]);
            build_tree(neighbor,curr,adj_list,people_arr);
        }
    }
}


int main(){
    int people_count;
    cin>>people_count;
    vector<node*>people_arr;
    for(int i = 0; i<people_count; i++){
        long long fun;
        cin>>fun;
        node* a = new node({},fun);
        people_arr.push_back(a);
    }
    vector<vector<int>> adj_list(people_count);
    for(int i = 1; i<people_count; i++){
        int a,b;
        cin>>a>>b;
        a--;b--;
        adj_list[a].push_back(b);
        adj_list[b].push_back(a);
    }


    int ceo = 0;
    build_tree(ceo,-1,adj_list,people_arr);
    cout<<fun_party(people_arr[ceo]);
}
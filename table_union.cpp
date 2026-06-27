#include<vector>
#include<iostream>
#include<algorithm>
using namespace std;


struct table{
    table(int s = 0):parent(this),rank(0),size(s){};
    table* parent;
    int rank;
    int size;
};


table* find(table* t){
    if (t->parent != t){
        t->parent = find(t->parent);
    }
    return t->parent;
}

void union_table(table& t1,table& t2,int& max_size){
    table* id_1 = find(&t1);
    table* id_2 = find(&t2);

    if (id_1 == id_2){
        return;
    }

    if (id_1->rank > id_2->rank){
        id_2->parent = id_1;
        id_1->size+=id_2->size;
        id_2->size = 0;
    }
    else{
        id_1->parent = id_2;
        id_2->size+=id_1->size;
        id_1->size = 0;
        if (id_1->rank == id_2->rank){
            id_2->rank += 1;
        }
    }
    max_size = max({max_size,id_1->size,id_2->size});
}

int main(){
    int max_size = 0;
    int table_count = 0;
    int merge_count = 0;
    vector<table>tables;
    vector<int>max_sizes;
    cin>>table_count>>merge_count;
    for(int i = 0;i<table_count;i++){
        int size = 0;
        cin >> size;
        max_size = max(max_size,size);
        tables.push_back(table(size));
    }
    for (int i = 0;i<table_count;i++){
        tables[i].parent = &tables[i];
    }
    for(int i = 0; i< merge_count;i++){
        int t1 = 0;
        int t2 = 0;
        cin>>t1>>t2;
        t1--;
        t2--;
        union_table(tables[t1],tables[t2],max_size);
        max_sizes.push_back(max_size);
    }
    for(int& i:max_sizes){
        cout<<i<<"\n";
    }

}



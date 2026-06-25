#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<string>

using namespace std;

struct stacked_object{
    stacked_object(int v):val(v){}
    long long val;
    stacked_object* next;
    stacked_object*prev;
};

class stack{
    public:
        stack()= default;
        vector<long long> max_values;
        stacked_object*head = nullptr;
        stacked_object* tail = nullptr;
        void push(long long v){
            stacked_object* new_object = new stacked_object(v);

            if (max_values.empty()){max_values.push_back(v);}
            else{
                long long last_max = max_values.back();
                if (last_max >= v ){max_values.push_back(last_max);}
                else{
                    max_values.push_back(v);
                }
            }
            if (tail == nullptr){
                head = new_object;
                tail = new_object;
            }
            else{
                tail->next = new_object;
                new_object->prev = tail;
                tail = new_object;
            }
        }
        void pop(){
            if (tail == nullptr){return;}
            max_values.pop_back();
            stacked_object*to_delete = tail;
            tail = tail->prev;
            if (tail != nullptr){
                tail->next = nullptr;
            }
            else{head = nullptr;}
            delete to_delete;
        }
        long long get_max(){
            return max_values.back();
        }

};

int main(){
    stack s = stack();
    queue<long long>ans;
    int op_count = 0;
    cin>>op_count;
    for(int i = 0;i<op_count;i++){
        string op = "";
        cin>>op;
        if (op == "push"){
            int v = 0;
            cin>>v;
            s.push(v);
        }
        else if(op == "pop"){s.pop();}
        else if(op == "max"){ans.push(s.get_max());}
    }
    while(!ans.empty()){
        cout<<ans.front()<<"\n";
        ans.pop();
    }
}
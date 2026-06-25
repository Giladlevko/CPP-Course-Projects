#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<string>

using namespace std;

struct stacked_object{
    stacked_object(int v):val(v),next(nullptr), prev(nullptr){}
    long long val;
    stacked_object* next;
    stacked_object*prev;
};

class stack{
    public:
        stack()= default;
        int size = 0;
        vector<long long> max_values;
        stacked_object*head = nullptr;
        stacked_object* tail = nullptr;
        void push(long long v){
            size++;
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
            size--;
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
            if (max_values.empty()){return 0;}
            return max_values.back();
        }
        bool empty(){return head == nullptr;}
        long long back(){return tail->val;}

};

class custom_queue{
    public:
        custom_queue() = default;
        int window_size;
        stack in_box;
        stack out_box;
        void push(long long v){in_box.push(v);}
        void pop(){
            if (out_box.empty()){
                while (!in_box.empty()){
                    out_box.push(in_box.back());
                    in_box.pop();
                } 
            }
            if(!out_box.empty()){
                out_box.pop();
            }
        }
        long long get_max(){
            return (max(in_box.get_max(),out_box.get_max()));
        }
};

int main(){
    custom_queue q = custom_queue();
    vector<long long>elements;
    int elm_count = 0;
    cin>>elm_count;
    for(int i = 0;i<elm_count;i++){
        long long a = 0;
        cin>>a;
        elements.push_back(a);
    }
    int w_size = 0;
    cin>>w_size;
    for(int i = 0;i<w_size;i++){
        q.push(elements[i]);
    }
    cout<<q.get_max()<<"\n";
    for(int i = w_size;i<elm_count;i++){
        q.pop();
        q.push(elements[i]);
        cout<<q.get_max()<<" ";
    }
    return 0;
}
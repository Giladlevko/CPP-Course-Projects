#include <iostream>
using namespace std;

class s_list{
    public:
        s_list():h(nullptr){}
        
        ~s_list(){
            cout << "destructor called"<<endl;
            release();
        }

        void prepend(const char*c){
            s_list_elem* temp = new s_list_elem;
            temp -> next = h;
            temp -> data = c;
            h = temp;//update h
        }

    private:
        struct s_list_elem{
            const char*data;
            s_list_elem* next;
        };
        s_list_elem* h;//list head

        void release(){
            s_list_elem* current = h;

            while(current != nullptr){
                s_list_elem* next = current->next;
                delete current;
                current = next;
            }
            h = nullptr;
        }
    friend ostream& operator<<(ostream& out, const s_list& list);
        
};

ostream& operator<<(ostream& out,const s_list& list){
    s_list::s_list_elem* current = list.h;
    while (current != nullptr){
        out<< current -> data<< " -> ";
        current = current -> next;
    }
    out << "nullptr";
    return out;
}

int main(){
    s_list list;
    list.prepend("A");
    list.prepend("B");
    list.prepend("C");

    cout << "done adding elements: "<<list<<endl;
    return 0;
}
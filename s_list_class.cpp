#include <iostream>
using namespace std;

class s_list{
    public:
        //normal constructor
        s_list():head(nullptr),cursor(nullptr){}
        
        //copy constructor
        s_list(const s_list&lst){
            if (lst.head == nullptr){head = nullptr;cursor = nullptr;}
            else{
                head = new s_list_elem(lst.head->data,nullptr);
                s_list_elem* current_node = head;
                s_list_elem* next_node = lst.head->next;

                while(next_node!=nullptr){
                    //create a new node for the copied list
                    //with the data of the next node to copy 
                    //from the original list
                    s_list_elem* new_node = new s_list_elem(next_node->data,nullptr);

                    //add the new node to the list
                    current_node->next = new_node;
                    //set the current node to the one just added
                    current_node = new_node;

                    //progress the next node
                    next_node = next_node->next;
                }
                cursor = head;
            }
        }
        s_list(int data[],int size){
            if (size == 0){head = nullptr;cursor = nullptr;}
            else{
                head = new s_list_elem(data[0],nullptr);
                s_list_elem* current_node = head;
                for (int i = 1; i<size;i++){
                    s_list_elem* new_node = new s_list_elem(data[i],nullptr);
                    current_node->next = new_node;
                    current_node = new_node;
                    
                }
                cursor = head;
            }
        }

        ~s_list(){
            cout << "destructor called"<<endl;
            release();
        }

        void prepend(int d){
            //empty list case
           if (head == nullptr){
            cursor = head = new s_list_elem(d,head);
           }
           //add to front
           else{
            head = new s_list_elem(d,head);
           }
        }

        int get_element(){
            if (cursor == nullptr){
                cout << "ERROR: NULL PTR"<<endl;
                return -1;}
            return cursor->data;
        }
        
        void advance(){
            if (cursor != nullptr){
                cursor = cursor-> next;
            }
        }


    private:
        struct s_list_elem{
            int data;
            s_list_elem* next;
            s_list_elem(int d, s_list_elem* n): data(d), next(n){}
        };
        s_list_elem* head;//list head
        s_list_elem* cursor;

        void release(){
            s_list_elem* current = head;

            while(current != nullptr){
                s_list_elem* next = current->next;
                delete current;
                current = next;
            }
            head = nullptr;
            cursor = nullptr;
        }
    friend ostream& operator<<(ostream& out, const s_list& list);
        
};

ostream& operator<<(ostream& out,const s_list& list){
    s_list::s_list_elem* current = list.head;
    while (current != nullptr){
        out<< current -> data<< " -> ";
        current = current -> next;
    }
    out << "nullptr";
    return out;
}

int main(){
    s_list list1;
    list1.prepend(1);
    list1.prepend(2);
    list1.prepend(3);
    s_list list2 = list1;
    int data[10] = {1,2,3,4,5,6};
    s_list list3(data,6);
    s_list list4(data,10);

    cout << "done adding elements: "<<list1<<endl;
    cout << "Copied list :\n"<<list2<<endl;
    cout<<"correct size: "<<list3<<endl;
    cout<<"incorrect size: "<<list4<<endl;
    

    return 0;
}
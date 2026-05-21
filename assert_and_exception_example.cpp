#include <cassert>//gives an assert at run time
#include <type_traits>
#include <iostream>
#include <exception>
using namespace std;
template <class T> 
class my_exception:public exception{
    private:
        T val;
    public:
    my_exception(T&a):val(a){}
    virtual const char* what()const throw(){
        if constexpr (is_pointer<T>::value){
            if (val == nullptr){
                return "My exception happened how could this be?";
            }
        }
        return "an Unknown error has accured.";

    }
};

template <class T> void swap(T&a,T&b){
    static_assert(//runs assert at compile time
    is_copy_constructible<T>::value,
    "Swap requires copying"
    );
    auto c = b;//expectation of copy construction
    b = a;
    a = c;
}


void print(int* val){
    assert (val!=nullptr);
    cout<<*val<<endl;
}

int main(){
    int i = 5;
    int* p = &i;
    int* q = nullptr;
    print(&i);
    print(p);
    
    //normal exception
    try{
        cout<<"test exception being caught\n";
        if (q == NULL){
            throw "this will abort";
        }
    }
    //universal signature works on anything
    catch(...){cout<<"Dont abort"<<endl;}
    //custom exception using the <exception>
    try{
        throw my_exception(q);
    }
    catch(exception& e){
        cout<<e.what()<<endl;
    }
    print(q);
    return 0;
}
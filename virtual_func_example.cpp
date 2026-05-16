#include <iostream>
using namespace std;

class B{
    public:
        int i = 5;
        virtual void print_i() const{cout<<i<<" Inside B"<<endl;}
};

class D: public B{
    public:
        void print_i() const{cout<<i<<" Inside D"<<endl;}//virtual
};

class E: public D{
    public:
        void print_i() const{cout<<i<<" Inside E"<<endl;}//still virtual
};

int main(){
    B b;
    B* pb = &b;//point at a B object
    D d;
    d.i = 1+(b.i = 1);
    pb -> print_i();//call B::print_i
    pb = &d;//point at a D object
    pb->print_i();//call D::print_i
    E e;
    e.print_i();
    return 0;
}
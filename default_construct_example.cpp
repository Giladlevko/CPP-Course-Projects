#include<iostream>
#include <string>
using namespace std;
class simple{
    public:
        simple() = default;//compiler generated
        simple(double x) = delete;//surpressed conversion constructor
        simple(int x, int y):p(x),q(y){}
        simple(int x):simple(x,1){}//delegate constructor
        simple(const simple& x):simple(x.p,x.q){}//delegate constructor
        simple(simple&& x):simple(x.p,x.q)
        {x.p = x.q = 0;}//move semantics

        friend ostream& operator<<(ostream& out,const simple& s){
            out<<"("<<s.p<<","<<s.q<<")";
            return out;
        }

    private:
        int p = 0,q = 1;//default initializer can be overriden
};

class rational:public simple{
    public:
        //implicitly declares constructors but uses base class
        using simple::simple;
};

int main(){
    simple s1 = 5;
    simple s2 = simple(simple(1,5));
    cout << s1<<":"<<s2<<endl;
    return 0;
}
#include <iostream>
#include <cmath>
using namespace std;

class point {
    public:
        point(double x=0.0,double y = 0.0):x(x),y(y){}//constructor
        double x,y;
        double getx()const{return x;}//inline
        void setx(double v){x = v;}//mutator

        double gety()const {return y;}
        void sety(double v){y = v;}

        double get_vector_size(){return(sqrt(x*x+y*y));}

        point operator+ (point& p2)const{
            point sum = {x+p2.x,y+p2.y};
            return sum;
        }
        point operator- (point& p2)const{
            point sub = {x-p2.x,y-p2.y};
            return sub;
        }
        point operator*( point& p2)const{
            point mult = {x*p2.x,y*p2.y};
            return mult;
        }
        point operator/(point& p2)const{
            point div = {x/p2.x,y/p2.y};
            return div;
        }
};

ostream& operator<< (ostream& out, const point& p){
    out << "("<<p.x<<", "<<p.y<<")";
    return out;
}

int main(){
    point a = {3.5,2.5};
    point b = {2.5,4.5};
    cout << "a = "<<a<<" b = "<<b<<endl;
    cout <<"sum = "<< a+b<<endl;
    cout <<"subtruction = "<< a-b<<endl;
    cout <<"multipication = "<< a*b<<endl;
    cout <<"division = "<< a/b<<endl;
    cout <<"vector size of a "<<a.get_vector_size()<<endl;
    cout <<"vector size of b "<<b.get_vector_size()<<endl;
    return 0;
}

#include <iostream>
#include<cmath>
using namespace std;


//tuple in C++ 11 or pair in c++
class duo{
    public:
        duo(double f = 0.0,double s = 0.0):first(f),second(s){}

        void set_first(double d){first = d;}
        void set_second(double d){second = d;}
        double get_first(){return first;}
        double get_second(){return second;}
    protected://Intermediate data hiding, gives access for inheritance
        double first,second;
};

class point:public duo{//a point is a form of duo
    public:
        point():duo(){}
        point(double x,double y = 0.0):duo(x,y){}
        //virtual is pure polymorphism and remains
       virtual double length(){return sqrt(first*first+second*second);}
};
//final makes it so there will be no further inheritance 
class point3d final:public point{
    public:
        point3d(double x = 0.0,double y = 0.0 ,double z = 0.0)
        :point(x,y),z(z){}
        //virtual
        double length(){return sqrt(first*first+second*second+z*z);}
        void set_z(double d){z = d;}
        double get_z(){return z;}
    private:
        double z;
};

int main(){
    point p;
    p.set_first(3.0);p.set_second(4.0);
    cout<<"2D Point: ";
    cout<< p.get_first()<<", "<<p.get_second()<<endl;
    cout<<"Length: "<< p.length()<<endl;//there is no length on duo
    point3d p_3d(3,4,5);
    cout<<"3D Point: ";
    cout<< p_3d.get_first()<<", "<<p_3d.get_second()
    <<", "<<p_3d.get_z()<<endl;
    cout<<"Length: "<< p_3d.length()<<endl;//different length
    return 0;
}
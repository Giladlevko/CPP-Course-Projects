#include <iostream>
#include <tuple>
using namespace std;

class threeD{
    public:
        threeD(double x = 0.0,double y = 0.0, double z = 0.0)
        {p = make_tuple(x,y,z);}//func from the tuple lib
        friend ostream& operator<<(ostream& out,threeD&d){
            out<<"("<<get<0>(d.p)<<","<<get<1>(d.p)<<","
            <<get<2>(d.p)<<")";
            //get gets the elememt with the inputed index 
            return out;
        }
    private:
        //tuple can also have different types in the same time
        tuple<double,double,double>p;     
};

int main(){
    threeD p1,p2(1.2,2.3,3.4);
    cout<<"Test Tuple"<<endl;
    cout<<"p1 is: "<<p1<<endl;
    cout<<"p2 is: "<<p2<<endl;
    return 0;
}
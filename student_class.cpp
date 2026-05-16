#include <string>
#include<iostream>
using namespace std;

enum year{fresh,soph,junior,senior,grad};
enum support{ta,ra,fellowship,other};

class student{
    public:
        
        student(string nm,int id,double g,year x): 
        name(nm),student_id(id),gpa(g),y(x){}
        void print() const{
            cout<<name<<", "<<student_id<<", "<<y<<", "<<gpa<<endl;
        }
    protected:
        int student_id;
        double gpa;
        year y;
        string name;
};

class grad_student:public student{//public-subtype
    public:
        
        grad_student(string nm,int id,double g,
            year x,support t,string d,string th):student(nm,id,g,x),
            s(t),thesis(th),dept(d){}
        void print() const{//overridden method
            student::print();
            cout<<dept<<", "<<s<<endl<<thesis<<endl;
        }
    protected:
            support s;
            string dept;
            string thesis;
};


int main(){//test pointer conversion rules
    student s("Mae Phol",100,3.425,fresh),*ps = &s;
    grad_student gs("Morris Phol",200,3.256,grad,
        ta,"Pharmacy","Retail Pharmacies"),*pgs;
    //ps is a student type and not a grad_student 
    //so it will always use the print in the student scope
    //even if it points to the address of a grad_student

    //pgs cannot point to vase class student only at grad_student

    //Using virtual for base functions makes it so the function used
    //is based on the object pointed at and not on the pointer type
    ps->print();//student::print
    ps = pgs = &gs;
    ps->print();//student::print
    pgs->print();//grad_student::print
    return 0;
}
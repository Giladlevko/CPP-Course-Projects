#include <string>

class student{
    public:
        enum year{fresh,soph,junior,senior,grad};
        student(string nm,int id,double g,year x): name(nm),student(id),
        gpa(g),y(x){}
        void print() const;
    protected:
        int student_id;
        double gpa;
        year y;
        string name;
};

class grad_student:public student{//public-subtype
    public:
        enum support{ta,ra,fellowship,other};
        grad_student(string nm,int id,double g,
            year x,support t,string d,string th): name(nm),student(id),
        gpa(g),y(x),s(t),dept(d),thesis(th){}
        void print() const;
    protected:
            support s;
            string dept;
            string thesis;
};
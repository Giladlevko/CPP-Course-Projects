#include <string>
#include<iostream>
#include<map>
using namespace std;
class Tree;

map<char,int> valtab;


class Node{
    friend class Tree;
    friend ostream& operator<<(ostream&,const Tree&);
    int use;//referential count -GC
    protected:
        Node(){use = 1;}
        
        virtual ~Node(){}//virtual destructor
        //pure virtual, they are to be overridden
        virtual void print(ostream&) = 0;
        virtual int eval() = 0;
};


class Tree{
    friend class Node;
    friend ostream& operator<<(ostream&,const Tree&);
    Node* p;//polymorphic heirarchy (sharing a common baseline)
    public:
        Tree(int);//constant

        Tree(char);//variable
           
        Tree(string,Tree);//unary operator

        Tree(string,Tree,Tree);//binary operator
  
        Tree(const Tree& t){//referential copy
            p = t.p;
            ++p->use;
        }
        ~Tree(){if(--p->use == 0){delete p;}}
        void operator=(const Tree& t);
        int eval(){return p->eval();}
};


class leaf_node:public Node{
    friend class Tree;
    void print(ostream& o) = 0;
    virtual int eval() = 0;
};


class int_node:public leaf_node{
    friend class Tree;
    int val;
    void print(ostream& o){o<<val;}
    int_node(int v):val(v){}
    public:
        int eval(){return val;}
};


class id_node:public leaf_node{
    friend class Tree;
    char name;
    void print(ostream& o){o<<name;}
    id_node(char n):name(n){}
    public:
        int eval(){return valtab[name];}
};


class unary_node:public leaf_node{
    friend class Tree;
    string op;
    Tree t;
    unary_node(string o,Tree tr):op(o),t(tr){}
    void print(ostream& o){
       o<<"("<<op<<t<<")";
    }
    public:

        int eval(){
            int result;
            if (op == "++"){result = t.eval()+1;}
            else if (op == "--"){result = t.eval()-1;}
            else if (op == "-"){result = -1*t.eval();}
            else if (op == "!"){result = !t.eval();}
            else{result = t.eval();}
            return result;
        }
};


class binary_node:public leaf_node{
    friend class Tree;
    string op;
    Tree left;
    Tree right;
    binary_node(string o,Tree l,Tree r):op(o),left(l),right(r){}
    void print(ostream& o){
        o<<"("<<left<<" "<<op<<" "<< right <<")";
    }
    public:
        int eval(){
            int result;
            if (op == "+"){result = left.eval()+right.eval();}
            else if(op == "-"){result = left.eval()-right.eval();}
            else if(op == "*"){result = left.eval()*right.eval();}
            else if(op == "/"){result = left.eval()/right.eval();}
            else{result = 0;}
            return result;
        } 
};

    Tree::Tree(int v){//constant
            p = new int_node(v);
    }
    Tree::Tree(char n){//variable
            p = new id_node(n);
    }
    Tree::Tree(string op,Tree t){//unary operator
            p = new unary_node(op,t);
    }
    Tree::Tree(string n ,Tree l,Tree r){//binary operator
            p = new binary_node(n,l,r);
    }


//polymorphic print
ostream& operator<<(ostream& o,const Tree& t){
    t.p->print(o);
    return (o);
}


int main(){
    valtab['b'] = 4;valtab['a'] = 3;
    cout<<"b = 4, a = 3";

    cout<<"Test 1 const node\n";
    Tree t1(6);
    cout<<"Expression: "<<t1<<endl;
    cout<<"Eval: "<<t1.eval()<<endl<<endl;
    cout<<"Test 2 var node\n";
    Tree t2('b');
    cout<<"Expression: "<<t2<<endl;
    cout<<"Eval: "<<t2.eval()<<endl<<endl;
    cout<<"Test 3 unary node\n";
    Tree t3("++",t2);
    cout<<"Expression: "<<t3<<endl;
    cout<<"Eval: "<<t3.eval()<<endl<<endl;
    cout<<"Test 4 binary node\n";
    Tree t4("*",t3,t1);
    cout<<"Expression: "<<t4<<endl;
    cout<<"Eval: "<<t4.eval()<<endl<<endl;

    cout <<"Nightmare expression from hell:\n";
    Tree t5("*",Tree("-",5),Tree("+",'a',4));
    Tree t6("+",Tree("-",'a',1),Tree("+",t5,'b'));
    cout<<"t5 =" <<t5<<"; t6 = "<<t6<<endl;
    cout <<"t5: "<<t5.eval()<<" t6: "<< t6.eval()<<endl;
    return 0;
}
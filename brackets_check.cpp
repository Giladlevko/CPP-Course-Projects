#include<string>
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;


struct bracket{
    char type;
    int indx;
};

void is_balanced(string str){
    vector<char> brackets = {'[',']','{','}','(',')'};
    vector<char> left_br = {'[','{','('};
    vector<bracket> stack;
    for (int i = 0; i<str.size();i++){
        char c = str[i];
        if (find(brackets.begin(),brackets.end(),c) != brackets.end()){
            if (find(left_br.begin(),left_br.end(),c)!=left_br.end()){
                stack.push_back({c,i+1});
            }
            else{
                if (stack.empty()){
                    cout<<i+1;
                    return;
                }
                else{
                    char top = stack.back().type;
                    stack.pop_back();
                    if ((top =='('&& c!=')') || (top =='['&&c!=']') || (top =='{'&&c!='}')){
                        cout<<i+1;
                        return;
                    }
                }
            }
        }
    }
    if (!stack.empty()){
        cout<<stack.front().indx;
        return;
    }
    cout<<"Success";
}


int main(){
    string a = "";
    cin>>a;
    is_balanced(a);
}
#include<vector>
#include<iostream>
#include<string>
#include<unordered_map>

using namespace std;

void add_entry(unordered_map<int,string>&book){
    int num = 0;
    string name ="";
    cin>> num;
    cin>>name;
    book[num] = name;
}

void del_entry(unordered_map<int,string>&book){
    int num = 0;
    cin>>num;
    book.erase(num);
}

string find_entry(unordered_map<int,string>&book){
    int num = 0;
    cin>>num;
    auto it = book.find(num);
    if(it!=book.end()){
        return book[num];
    }
    return "not found";
}


int main(){
    unordered_map<int,string> phone_book;
    vector<string>ans;
    int ops = 0;
    cin>>ops;
    for (int i = 0; i<ops;i++){
        string a = "";
        cin>>a;
        if (a == "add"){
            add_entry(phone_book);
        }
        if (a == "del"){
            del_entry(phone_book);
        }
        if (a=="find"){
            string answer = find_entry(phone_book);
            ans.push_back(answer);
        }

    }
    for(string& a:ans){
        cout<<a<<"\n";
    }


    return 0;
}
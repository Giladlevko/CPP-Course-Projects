#include<list>
#include<vector>
#include<algorithm>
#include<string>
#include<iostream>

using namespace std;

class custom_dict{
    private:
        int list_count;
        vector<list<string>>lists;

        long long hash(const string&s){
            long long hash_val = 0;
            long long p = 1000000007;
            long long x = 263;
            for(int i = s.length()-1;i>=0;i--){
                hash_val = (hash_val*x+s[i])%p;
            }
            return hash_val % list_count;
        }
    public:
        custom_dict(int size):list_count(size),lists(size){}

        void add(const string& s){
            long long hash_val = hash(s);
            list<string>& chain = lists[hash_val];
            auto it = find(chain.begin(),chain.end(),s);
            if(it == chain.end()){
                chain.push_front(s);
            }
        }

        void del(const string& s){
            long long hash_val = hash(s);
            list<string>& chain = lists[hash_val];
            auto it = find(chain.begin(),chain.end(),s);
            if(it != chain.end()){
                chain.erase(it);
            }
        }

        void find_str(const string& s){
            long long hash_val = hash(s);
            const list<string>& chain = lists[hash_val];
            auto it = find(chain.begin(),chain.end(),s);
            if(it != chain.end()){
                cout<< "yes\n";
            }
            else{cout<<"no\n";}
        }
        void check(int i){
            const list<string> chain = lists[i];
            for(const string& s:chain){
                cout<<s<<" ";
            }
            cout<<"\n";
        }
};

int main(){
    int size = 0,op = 0;
    cin>>size>>op;
    custom_dict dict(size);
    for(int i = 0;i<op;i++){
        string a = "";
        cin>>a;
        if (a == "find"){
            string s = "";
            cin>>s;
            dict.find_str(s);
        }
        if (a=="add"){
            string s = "";
            cin>>s;
            dict.add(s);
        }
        if (a=="del"){
            string s = "";
            cin>>s;
            dict.del(s);
        }
        if(a=="check"){
            int i = 0;
            cin>>i;
            dict.check(i);
        }
    }
}
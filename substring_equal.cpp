#include<vector>
#include<iostream>
#include<string>

using namespace std;

class substring_equality{
    private:
        const long long p1 = 1000000007;
        const long long p2 = 1000000009;
        const long long x1 = 263;
        const long long x2 = 811;

        vector<long long> h1,h2;
        vector<long long> power1,power2;
    public:
        substring_equality(const string & s){
            int n = s.size();
            h1.assign(n+1,0);
            h2.assign(n+1,0);
            power1.assign(n+1,1);
            power2.assign(n+1,1);
            for(int i = 1;i<=n;i++){
                h1[i] = (h1[i-1]*x1 + s[i-1]) % p1;
                h2[i] = (h2[i-1]*x2 + s[i-1]) % p2;
                power1[i] = (power1[i-1]*x1)%p1;
                power2[i] = (power2[i-1]*x2)%p2;
            }
        }

        bool is_equal(int a,int b,int l){
            if(l==0){return true;}
            long long hash_a1 = (h1[a+l] - h1[a]*power1[l])%p1;
            if(hash_a1<0){hash_a1+=p1;}

            long long hash_a2 = (h2[a+l] - h2[a]*power2[l])%p2;
            if(hash_a2<0){hash_a2+=p2;}

            long long hash_b1 = (h1[b+l] - h1[b]*power1[l])%p1;
            if(hash_b1<0){hash_b1+=p1;}
            
            long long hash_b2 = (h2[b+l] - h2[b]*power2[l])%p2;
            if(hash_b2<0){hash_b2+=p2;}

            return (hash_a1 == hash_b1 && hash_a2 == hash_b2);
        }
};

int main(){
    string s = "";
    int q = 0;
    cin>>s>>q;

    substring_equality equality(s);
    
    for(int i = 0;i<q;i++){
        int a = 0,b = 0,l = 0;
        cin>>a>>b>>l;
        if (equality.is_equal(a,b,l)){
            cout<<"Yes\n";
        }
        else{cout<<"No\n";}
    }
    return 0;
}
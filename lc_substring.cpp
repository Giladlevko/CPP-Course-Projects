#include<vector>
#include<iostream>
#include<string>
#include<algorithm>

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

        pair<long long,long long> get_hash(int a,int l){
            if(l==0){return {0,0};}
            long long hash_a1 = (h1[a+l] - h1[a]*power1[l])%p1;
            if(hash_a1<0){hash_a1+=p1;}

            long long hash_a2 = (h2[a+l] - h2[a]*power2[l])%p2;
            if(hash_a2<0){hash_a2+=p2;}

            return {hash_a1, hash_a2};
        }
};

struct answer{
    answer(int s1 = 0 ,int s2 = 0,int l = 0):start_1(s1),start_2(s2),len(l){}
    int start_1,start_2,len;
};

bool check_length(int l, int len_1, int len_2, substring_equality& solver_1, substring_equality& solver_2, answer& best){
    if (l == 0) return true;
    vector<pair<pair<long long ,long long>,int>> hashes_1;
    for(int i = 0;i<=len_1-l;i++){
        hashes_1.push_back({solver_1.get_hash(i,l),i});
    }
    sort(hashes_1.begin(),hashes_1.end());

    for (int i = 0;i<=len_2-l;i++){
        pair<long long , long long> target_hash = solver_2.get_hash(i,l);
        pair<pair<long long , long long>,int> target = {target_hash,0};
        auto it = lower_bound(hashes_1.begin(),hashes_1.end(),target);
        if (it != hashes_1.end() && target_hash == it->first){
            best.start_1 = it->second;
            best.start_2 = i;
            best.len = l;
            return true;
        }
        
    }
    return false;
}

answer lc_substring(const string& s1,const string& s2){
    int len_1 = s1.length();
    int len_2 = s2.length();
    substring_equality solver_1(s1);
    substring_equality solver_2(s2);
    int low = 0;
    int high = min(len_1,len_2);
    answer best(0,0,0);
    while(high>=low){
        int mid = low +(high-low)/2;
        if (check_length(mid,len_1,len_2,solver_1,solver_2,best)){
            low = mid+1;
        }
        else{
            high = mid -1;
        }
    }
    return best;
}

int main(){
    string s1 = "",s2 = "";
    while(cin>>s1>>s2){
        answer ans = lc_substring(s1,s2);
        cout<<ans.start_1<<" "<<ans.start_2<<" "<<ans.len<<"\n";
    }
    return 0;
}
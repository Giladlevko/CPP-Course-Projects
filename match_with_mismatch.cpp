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


int lc_prfx(int curr_t, int curr_p, int max_possible_len,substring_equality& solver_1,substring_equality& solver_2){

    int low = 1;
    int high = max_possible_len;

    int longest_prefix = 0;

    while(high>=low){
        int mid = low +(high-low)/2;
        if (solver_1.get_hash(curr_t,mid) == solver_2.get_hash(curr_p,mid)){
            low = mid+1;
            longest_prefix = mid;
        }
        else{
            high = mid -1;
        }
    }
    return longest_prefix;
}


vector<int> matching_pos_with_mismatch(const int& max_mismatch,const string& txt,const string& pattern){
    substring_equality solver_1(txt);
    substring_equality solver_2(pattern);
    vector<int> answers;
    int txt_size = txt.size();
    int pat_size = pattern.size();
    for(int i = 0;i <= txt_size - pat_size ;i++){
        int misses = 0;
        int cur_prfx = 0;
        while(cur_prfx<pat_size){
            int cur_t_pos = i + cur_prfx;
            int remaining_chars = pat_size-cur_prfx;
            int lcp = lc_prfx(cur_t_pos,cur_prfx,remaining_chars,solver_1,solver_2);
            cur_prfx += lcp;
            if (cur_prfx < pat_size){
                misses++;
                if(misses>max_mismatch){
                    break;
                }
                cur_prfx++;
            }

        }
        if (misses<=max_mismatch){
            answers.push_back(i);
        }
    }
    return answers;
}

int main(){
    int max_mismatch = 0;
    string t = "",p = "";
    while(cin>>max_mismatch>>t>>p){
        vector<int> ans = matching_pos_with_mismatch(max_mismatch,t,p);
        cout<<ans.size()<<" ";
        for (int& i:ans){
            cout<<i<<" ";
        }
        cout<<"\n";

    }
    return 0;
}
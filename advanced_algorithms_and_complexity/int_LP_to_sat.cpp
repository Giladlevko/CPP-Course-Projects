#include<vector>
#include<iostream>

using namespace std;

struct inequality{
    inequality(vector<long long>a,long long b = 0):arr(a),bound(b){}
    inequality() = default;
    long long bound;
    vector<long long> arr;
};

struct variable{
    variable(int i,long long c):index(i),coeff(c){}
    variable() = default;
    int index;
    long long coeff;
};

//maps to a sat index from x_i 
int sat_index(int x_i){
    return (x_i + 1);
}


vector<vector<int>> int_linear_programming_to_sat(
    vector<inequality>&inequalities
){
    vector<vector<int>> clauses;
    for(const auto& inequality:inequalities){
        int size = inequality.arr.size();
        vector<variable> non_zeros;
        for(int i = 0; i<size; i++){
            int coeff = inequality.arr[i];
            if(coeff != 0){
                non_zeros.push_back(variable(i,coeff));
            }
        }
        int not_zero_count = non_zeros.size();
        //if all are zero and the bound is negative -> 0+0+0 <= -1
        //will never be true so add contradictory clauses
        if(not_zero_count == 0 && inequality.bound < 0){
            clauses.push_back({1});clauses.push_back({-1});
        }

        int comb_count = (1<<not_zero_count);

        for(int mask = 0; mask<comb_count; mask++){
            long long sum = 0;
            for(int j = 0; j<not_zero_count; j++){
                //looks at the j'th bit in the mask and sees if its on (1) or off (0)
                //i.e if mask is 3 or 011 in binary and j = 1
                //it sees that at position 1 (0 <1> 1) it is on
                //the way it does this is by first shifting all bits
                //j amont to the right meaning the j'th bit will now be at bit 0
                //then it compares it bit by bit to 1 or 001 in binary
                //if those two bits have 1 in the same position => it returns 1
                bool is_on = (mask>>j) & 1;
                if(is_on){sum += non_zeros[j].coeff;}
            }
            if(sum > inequality.bound){
                vector<int>clause;
                //cant be accepted in 1 bits and not accepted in 0 bits
                //if mask = 110 -> -(-x_1 ^ x_2 ^ x_3) => (x_1 v -x_2 v x_3)
                //so if on set as - if off set as +
                for(int j = 0; j<not_zero_count; j++){
                    int id = sat_index(non_zeros[j].index);

                    //same as before
                    bool is_on = (mask>>j) & 1;

                    if(is_on){ id *= -1; }

                    clause.push_back(id);
                }
                clauses.push_back(clause);
            }
        }
    }
    return clauses;
}


int main(){
    int inequality_count, var_count;
    cin>>inequality_count>>var_count;
    vector<inequality>inequalities(inequality_count);
    for(int i = 0; i<inequality_count; i++){
        vector<long long> arr(var_count);
        for(int j = 0; j<var_count; j++){
            cin>>arr[j];
        }
        inequalities[i] = inequality(arr);
    }
    for(int i = 0; i<inequality_count; i++){
        cin>>inequalities[i].bound;
    }
    vector<vector<int>>clauses = int_linear_programming_to_sat(inequalities);
    
    //if no clauses add a satisfing clause like
    // x1 or -x1 which is always true;
    if (clauses.empty()){
       clauses.push_back({1,-1});
    }
    int clause_count = clauses.size();
    cout<<clause_count<<" "<<var_count<<"\n";
    for(int i = 0; i<clause_count; i++){
        for(int j = 0; j<clauses[i].size(); j++){
            cout<<clauses[i][j]<<" ";
        }
        cout<<"0\n";
    }
    return 0;
}

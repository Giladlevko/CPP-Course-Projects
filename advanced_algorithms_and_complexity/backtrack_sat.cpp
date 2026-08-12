#include<vector>
#include<iostream>
#include<algorithm>

using namespace std;

enum class clause_state{UNKNOWN,SAT,UNSAT};


clause_state eval_clause(const vector<int>&clause,const vector<int>& assignment){
    bool has_unknown = false;
    for(const int& x_i:clause){
        //if has one var that we still dont know we cant know the state of the clause
        if(assignment[abs(x_i)] == 0){has_unknown = true;continue;}
        if(x_i < 0 && assignment[abs(x_i)] == -1){
            return(clause_state::SAT);
        }
        if(x_i>0 && assignment[x_i] == 1){
            return(clause_state::SAT);
        }
    }
    if(has_unknown){return clause_state::UNKNOWN;}
    return(clause_state::UNSAT);
}


bool solve_sat(
    vector<vector<int>>& clauses,
    vector<int>&assignment,
    int var_count,int curr_var = 1
){
    bool all_sat = true;
    for(const vector<int>&clause:clauses){
        clause_state state = eval_clause(clause,assignment);
        //if one clause is unsat then it means that the formula is unsat
        if(state == clause_state::UNSAT){return false;}
        if(state == clause_state::UNKNOWN){
            //still at least one clause we are unsure about
            all_sat = false;break;
        }
    }
    if(all_sat){return true;}
    if(curr_var>var_count+1){return false;}

    int x = curr_var++;//moving forward from 1 to the last var

    assignment[x] = 1;
    if(solve_sat(clauses,assignment,var_count,curr_var)){
        return true;
    }
    assignment[x] = -1;
    if(solve_sat(clauses,assignment,var_count,curr_var)){
        return true;
    }
    //undo the changes for the backtrack
    assignment[x] = 0;
    return false;
}


void handle_sat(vector<vector<int>>& clauses,const int &var_count){
    vector<int>result(var_count+1,0);
    bool sat = solve_sat(clauses,result,var_count);
    if(sat){
        cout<<"SATISFIABLE\n";
        for(int i = 1; i<=var_count; i++){
            cout<<(i)*result[i]<<" ";
        }
    }
    else{
        cout<<"UNSATISFIABLE";
    }
    
}


int main(){
    int vars,clause_count;
    cin>>vars>>clause_count;
    vector<vector<int>>clauses(clause_count);
    for(int i = 0; i<clause_count; i++){
        int x_i,x_j;
        cin>>x_i>>x_j;
        clauses[i] = {x_i,x_j};
    }
    handle_sat(clauses,vars);
}
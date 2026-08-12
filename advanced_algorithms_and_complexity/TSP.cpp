#include<vector>
#include<iostream>

using namespace std;


//I used this video for assistance 
// it explains each stage of this problem very well:
//https://youtu.be/cY4HiiFHO1o?si=nIGM7ws5NYINnQtv

const long long INF = 1e17;

void setup(
    vector<vector<long long>>&memo,const vector<vector<long long>>& tsp_m,
    const int& start,const int& k
){
    for(int i = 0;i<k;i++){
        if(i==start){continue;}
        //compines 1 at start'th pos and 1 at i'th pos into one
        //so it gives the dist from start to i
        memo[i][1 << start | 1 << i] = tsp_m[start][i];
    }
}

//returns true if the i'th bit in the subset is not there
bool not_in(int i,int subset){
    return((1 << i & subset) == 0);
}

void gen_combinations(int set,int at,int r,int k,vector<int>& sets){
    if(r == 0){
        sets.push_back(set);
    }
    else{
        for(int i = at;i<k;i++){
            //flip on i'th bit
            set = set | 1 << i;

            gen_combinations(set,i+1,r-1,k,sets);

            //backtrack and flip off i'th bit
            set = set & ~(1 << i);
        }
    }
}

//generates all bit sets of size k with r bits set to 1
//combinations(3,4) = [0111, 1011, 1101, 1110]
vector<int> combinations(int r,int k){
    vector<int> sets;
    gen_combinations(0,0,r,k,sets);
    return sets;

}

void solve(
    vector<vector<long long>>&memo,
    const vector<vector<long long>>& tsp_m,
    const int& start,const int& k
){
    for(int r = 3;r<=k;r++){
        vector<int> subsets = combinations(r,k);
        for(int& subset:subsets){
            if(not_in(start,subset)){continue;}
            for(int next = 0; next<k; next++){
                if(next == start || not_in(next,subset)){continue;}
                //the subset state without the next node
                //essentially turns off the next'th bit in the subset
                int state = subset ^ (1 << next);
                long long min_dist = INF;
                int best_end;

                for(int end_node = 0; end_node < k; end_node++){
                    if(end_node == start || end_node == next || not_in(end_node,subset)){
                        continue;
                    }
                    //checks the dist up until I arrived to the next node
                    //then adds the distance from the last node i was in to the next node
                    //giving me a correct distance for the current path
                    long long new_dist = memo[end_node][state] + tsp_m[end_node][next];
                    if (new_dist < min_dist){
                        min_dist = new_dist;
                        best_end = end_node;
                    }
                }
                memo[next][subset] = min_dist;
            }
        }
    }

}

long long get_min_cost(
    vector<vector<long long>>&memo,
    const vector<vector<long long>>& tsp_m,
    const int& start,const int& k,int& best_end
){
    //the END_STATE is the bit mask with k bits set to one -> 2^k - 1
    const int END_STATE = (1 << k) -1;
    long long min_tour_cost = INF;
    for(int end_node = 0; end_node<k; end_node++){
        if(end_node == start){continue;}
        //looks at all the possible end nodes for when all the nodes have been visited
        //and sees which end node gives the min distance for the route + the journey from the end to the start
        long long tour_cost = memo[end_node][END_STATE] + tsp_m[end_node][start];
        if(tour_cost<min_tour_cost){
            min_tour_cost = tour_cost;
            best_end = end_node;
        }
    }
    if(min_tour_cost == INF){min_tour_cost = -1;}
    return min_tour_cost;
}

vector<int> find_optimal_path(
    vector<vector<long long>>&memo,
    const vector<vector<long long>>& tsp_m,
    const int& start,const int& k
){
    vector<int>tour(k+1);
    int last_indx = start;
    int state = (1<<k) -1;//end state
    for(int i = k-1; i>=1; i--){
        int index = -1;
        for(int j = 0; j<k; j++){
            if(j == start || not_in(j,state)){continue;}
            if(index == -1){index = j;}
            long long prev_dist = memo[index][state] + tsp_m[index][last_indx];
            long long new_dist = memo[j][state] + tsp_m[j][last_indx];
            if(new_dist < prev_dist){index = j;}
        }
        tour[i] = index;
        state = state ^ (1<<index);
        last_indx = index;
    }
    tour[0] = tour[k] = start;
    return tour;
}


void tsp_dp(const vector<vector<long long>>& tsp_m,const int& start){
    int k = tsp_m.size();
    vector<vector<long long>>memo(k,vector<long long>((1 << k),-1));//2^k column size
    setup(memo,tsp_m,start,k);
    solve(memo,tsp_m,start,k);
    int best_end;
    long long min_cost = get_min_cost(memo,tsp_m,start,k,best_end);

    if(min_cost == -1){cout<<-1;return;}

    cout<<min_cost<<"\n";

    vector<int> path = find_optimal_path(memo,tsp_m,start,k);
    for(int i = 0; i<k; i++){
        cout<<path[i]+1<<" ";//back to 1 indexed
    }
}


int main(){
    int v_count,e_count;
    cin>>v_count>>e_count;
    vector<vector<long long>>matrix(v_count,vector<long long>(v_count,INF));
    for(int i = 0; i<e_count; i++){
        int u,v,t;
        cin>>u>>v>>t;
        u--;v--;//to 0 index
        matrix[u][v] = t;
        matrix[v][u] = t;
    }
    int start = 0;
    tsp_dp(matrix,start);
}
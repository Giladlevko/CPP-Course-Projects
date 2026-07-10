#include<vector>
#include<queue>
#include<iostream>
using namespace std;

class color{
    private:
        enum class colors{BLACK = -1,WHITE = 1};
        
    public:
        color(colors c):type(c){}
        color(int i):type(static_cast<colors>(i)){}
        colors type;
        colors oppisite(){
            int val = static_cast<int>(type);
            return static_cast<colors>(-1*val);
        }

};

void is_bipartite(bool is){
    cout << is;
}

bool bfs(const vector<vector<int>>& edges,const int& start_node,vector<int>&dist,vector<color>&colors){
    dist[start_node] = 0;
    queue<int> q;
    q.push(start_node);
    colors[start_node] = color(1);
    int curr;
    while(!q.empty()){
        curr = q.front();
        q.pop();
        for(const int& v:edges[curr]){
            if(dist[v] == -1){
                q.push(v);
                dist[v] = dist[curr]+1;
                colors[v] = colors[curr].oppisite();
            }
            else if(colors[v].type == colors[curr].type){
                return false;
            }
        }
    }
    return true;
}
void check_bipartite(const vector<vector<int>>& edges,vector<int>&dist,vector<color>&colors){
    int vert_count = edges.size();
    for(int i = 1;i<vert_count;i++){
        if (dist[i] == -1){
            bool result = bfs(edges,i,dist,colors);
            if (result == false){
                is_bipartite(false);
                return;
            }
        }
    }
    is_bipartite(true);
}

int main(){
    int e,v;
    cin>>v>>e;
    vector<int> dist(v+1,-1);//-1 means unreachable
    vector<vector<int>>edges(v+1);
    vector<color>colors(v+1,1);
    for(int i = 0;i<e;i++){
        int a,b;
        cin>>a>>b;
        edges[a].push_back(b);
        edges[b].push_back(a);
    }
    check_bipartite(edges,dist,colors);
    return 0;
}
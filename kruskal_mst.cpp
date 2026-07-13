#include<vector>
#include<iostream>
#include <iomanip>
#include<algorithm>
#include<cmath>
using namespace std;


struct point{
    point(int x = 0,int y = 0):x(x),y(y),parent(this),rank(0),size(1){};
    point* parent;
    int rank;
    int size;
    int x;
    int y;
};


point* find(point* t){
    if (t->parent != t){
        t->parent = find(t->parent);
    }
    return t->parent;
}

void union_point(point* t1,point* t2){
    point* id_1 = find(t1);
    point* id_2 = find(t2);

    if (id_1 == id_2){
        return;
    }

    if (id_1->rank > id_2->rank){
        id_2->parent = id_1;
        id_1->size+=id_2->size;
        id_2->size = 0;
    }
    else{
        id_1->parent = id_2;
        id_2->size+=id_1->size;
        id_1->size = 0;
        if (id_1->rank == id_2->rank){
            id_2->rank += 1;
        }
    }
}

struct edge{
    edge(point* f,point* t):from(f),to(t){}
    point* from;
    point* to;
};

long double length(point*a,point*b){
    return sqrt(((a->x - b->x) * (a->x - b->x)) + ((a->y - b->y) * (a->y - b->y)));
}

bool sort_by_length(const edge& a,const edge& b){
    if (length(a.from,a.to) < length(b.from,b.to)){
        return true;
    }
    return false;
}

long double kruskal(vector<edge>&edges){
    long double total_length = 0;
    sort(edges.begin(),edges.end(),sort_by_length);
    for(const edge& e:edges){
        if(find(e.from) != find(e.to)){
            union_point(e.from,e.to);
            total_length += length(e.from,e.to);
        }
    }
    return total_length;
}

vector<edge> connect_points(const vector<point*>&points){
    vector<edge>edges;
    int size = points.size();
    for(int i = 0;i<size;i++){
        for(int j = i+1;j<size;j++){
            edges.push_back(edge(points[i],points[j]));
            }
        }
    return edges;
}

int main(){
    cout << fixed << setprecision(9);
    int point_count;
    vector<point*>points;
    cin>>point_count;
    for(int i = 0;i<point_count;i++){
        int x,y;
        cin>>x>>y;
        point* point_a = new point(x,y);
        points.push_back(point_a);
    }
    vector<edge> all_edges = connect_points(points);

    cout<<kruskal(all_edges);
}

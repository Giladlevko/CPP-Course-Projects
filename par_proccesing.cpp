#include<vector>
#include<iostream>
#include<utility>

using namespace std;

struct thread{
    thread(int i):id(i),free_time(0){}
    int id;
    long long free_time;
};

struct job{
    job(int t):proccess_time(t){}
    int proccess_time;
};


int parent(const int& i){
    return((i-1)/2);
}
int right_child(const int& i){
    return(2*i+2);
}
int left_child(const int& i){
    return(2*i+1);
}

void sift_down(int i,vector<thread>& arr){
    int size = arr.size();
    int max_indx = i;
    int l = left_child(i);
    int r = right_child(i);
    if (l<size && ((arr[l].free_time < arr[max_indx].free_time) ||
     (arr[l].free_time == arr[max_indx].free_time) && arr[l].id < arr[max_indx].id)){
        max_indx = l;
    }
    if (r<size && ((arr[r].free_time < arr[max_indx].free_time) ||
     (arr[r].free_time == arr[max_indx].free_time) && arr[r].id < arr[max_indx].id)){
        max_indx = r;
    }
    if (i != max_indx){
        thread temp = arr[i];
        arr[i] = arr[max_indx];
        arr[max_indx] = temp;
        sift_down(max_indx,arr);
    }
}


void build_heap(vector<thread>& arr){
    int size = arr.size()-1;
    for(int i = size/2;i>=0;i--){
        sift_down(i,arr);
    }
}

int main(){
    vector<thread> threads;
    vector<long long> job_durations;
    int t_size = 0;
    int j_size = 0;

    cin>>t_size>>j_size;

    for(int i = 0; i<t_size;i++){
        threads.push_back(thread(i));
    }

    for(int i = 0;i<j_size;i++){
        long long a = 0;
        cin>>a;
        job_durations.push_back(a);
    }

    build_heap(threads);

    for(int i = 0;i<j_size;i++){
        cout<<threads[0].id<<" "<<threads[0].free_time<<endl;
        threads[0].free_time+=job_durations[i];
        sift_down(0,threads);
    }

    return 0;
}
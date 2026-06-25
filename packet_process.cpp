#include<vector>
#include<queue>
#include<iostream>
using namespace std;

class packet{
    public:
        packet(int t1,int t2):arrive_time(t1),procces_time(t2){}
        int arrive_time;
        int procces_time;
        int finish_time;
};

void process_packets(vector<packet> v,int b_size){
    int packet_count = v.size();
    queue<int>buffer;
    for (int i = 0;i<packet_count;i++){
        packet& p = v[i];
        while(!buffer.empty() && buffer.front()<=p.arrive_time){
            buffer.pop();
        }

        if (buffer.size()==b_size){
            cout<<(-1)<<endl;
        }
        else{
            p.finish_time = p.arrive_time+p.procces_time;
            if (!buffer.empty()){
                p.finish_time = buffer.back()+p.procces_time;
            }
            int start_time = p.finish_time-p.procces_time;
            cout<<start_time<<endl;
            buffer.push(p.finish_time);
        }

    }
}

int main(){
    vector<packet> packets;
    int buffer_size = 0;
    int packet_count = 0;
    cin>>buffer_size>>packet_count;
    for(int i = 0;i<packet_count;i++){
        int arrive_time = 0;
        int process_time = 0;
        cin>>arrive_time>>process_time;
        packets.push_back({arrive_time,process_time});
    }
    process_packets(packets,buffer_size);
    return 0;
}
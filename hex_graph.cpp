

class hex_graph{
    public:
        hex_graph(){
            int count = 0;
            edge_list.resize(121);
            for (int i = 0;i<11;1++){
                for(int j = 0; j<11;j++){
                    make_node(i,j,edge_list[count++]);
                }
            }
        }
    private:
        //deque could be vector as well
        vector<deque<int>> edge_list;
};
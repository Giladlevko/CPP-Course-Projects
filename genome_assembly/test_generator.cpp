#include<vector>
#include<random>
#include<string>
#include<fstream>
#include<algorithm>
#include<iostream>

using namespace std;

const int gene_len = 500000;
const int GC_percentage = 50;
const int read_len = 100;
const int coverage = 12;
const string GC_bases = "CG";
const string AT_bases = "AT";
const string bases = "ACGT";


char get_rand_base(mt19937& rng){
    uniform_int_distribution<int>rand_perc(0,100);
    uniform_int_distribution<int>rand_index(0,1);
    if(rand_perc(rng)<=GC_percentage){
        return GC_bases[rand_index(rng)];
    }
    else{
        return AT_bases[rand_index(rng)];
    }

}


void gen_rand_genome(string& gene){
    
    mt19937 rng(100);
    uniform_int_distribution<int>dist(0,3);
    for(int i = 0; i<gene_len; i++){
        gene += get_rand_base(rng);
    }
}

void diff_rand_base(char&base,mt19937& rng){
    string shuffled_bases = bases;
    shuffle(shuffled_bases.begin(),shuffled_bases.end(),rng);
    for(const char& c: shuffled_bases){
        if(c != base){
            base = c;
            return;
        }
    }
}

void break_gene_into_reads(string& gene,vector<string>&reads){
    mt19937 rng(100);
    uniform_int_distribution<int>dist(0,gene.size()-read_len);
    int read_count = gene.size() * coverage / read_len;
    for(int i = 0; i<read_count; i++){
        int start = dist(rng);
        string read = gene.substr(start,read_len);
        //introduce a 1% error
        uniform_int_distribution<int>read_dist(0,read.size()-1);
        int error_i = read_dist(rng);
        diff_rand_base(read.at(error_i),rng);
        reads.push_back(read);
    }
}

void write_rand_test_to_file(){
    string gene = "";
    gen_rand_genome(gene);
    ofstream gene_file("genome_assembly/reference_gene.txt");
    gene_file<<">REFERENCE GENOME\n"<<gene;
    vector<string>reads;
    break_gene_into_reads(gene,reads);
    ofstream file("genome_assembly/test_inputs.txt");
    if(!file){cout<<"could not open file\n";}

    cout<<"Writing to file: genome_assembly/test_inputs.txt\ngenome size: "<<gene_len
    <<"\nread size: "<<reads[0].size()<<"\nread count: "<<reads.size()<<"\nerror percentage: 1%"
    <<"\ncoverage: "<<coverage << "\nGC%: "<<GC_percentage;
    
    file<<reads.size()<<"\n";
    
    for(int i = 0; i<reads.size(); i++){
        file<<reads[i]<<"\n";
    }
    file.close();
}

int main(){
    write_rand_test_to_file();
    return 0;
}
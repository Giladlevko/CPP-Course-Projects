#include <iostream>
#include <vector>
#include<algorithm>
#include<cstdlib>
#include <cassert>
using namespace std;

enum class suit:short{SPADE,HEART,DIAMOND,CLUB};

class pips{
    public:
        pips(int val):v(val){assert(v>0 && v<14);}
        friend ostream& operator<<(ostream& out, const pips& p);
        int get_pips()const{return v;}
    private:
        int v;
};

class card{
    public:
        card():s(suit::SPADE),v(1){}
        card(suit s, pips v):s(s),v(v){}
        friend ostream& operator<<(ostream& out,const card& c);
        suit get_suit()const{return s;}
        pips get_pips()const{return v;}
    private:
        suit s;
        pips v;
};

ostream& operator<<(ostream& out, const pips& p){
    int val = p.get_pips();
    switch(val){
        case 13: out<<"King";break;
        case 12: out<<"Queen";break;
        case 11: out<<"Jack";break;
        case 1: out<<"Ace";break;
        default: out<<val;break;
    }
    return out;
}

ostream& operator<<(ostream& out, const suit& s){
    switch(s){
        case suit::SPADE: out<<"Spades";break;
        case suit::HEART: out<<"Hearts";break;
        case suit::DIAMOND: out<<"Diamonds";break;
        case suit::CLUB: out<<"Clubs";break;
        default: out<<"ERORR: SUIT NOT FOUND";break;
    }
    return out;
}

ostream& operator<<(ostream& out, const card& c){
    out<<c.v<<" of "<<c.s;
    return out;
}

void init_deck(vector<card>&d){
    //suit s_type = suit::SPADE
    for(int i = 1;i<14;i++){
        card c(suit::SPADE,i);
        d[i-1]=c;
    }
    for(int i = 1;i<14;i++){
        card c(suit::HEART,i);
        d[i+12]=c;
    }
    for(int i = 1;i<14;i++){
        card c(suit::DIAMOND,i);
        d[i+25]=c;
    }
    for(int i = 1;i<14;i++){
        card c(suit::CLUB,i);
        d[i+38]=c;
    }

}

void print_deck(const vector<card>&deck){
    for(auto card_val:deck){
        cout << card_val<<": ";
    }
    cout<<endl;
}

bool is_flush(const vector<card>& hand){
    suit s = hand[0].get_suit();
    for(auto card_val:hand){
        if (s!=card_val.get_suit()){
            return false;
        } 
    }
    return true;
}

bool is_sequential(const int* hand,int start_i = 0){
    int last_pip = -1;
    const int HAND_SIZE = 5;
    for (int i =start_i;i<HAND_SIZE;i++ ){
        int card_pip = hand[i];
        if (last_pip!=-1){
            if ((card_pip - 1) != last_pip){
                return false;
            }
        }
        last_pip = card_pip;
    }
    return true;
}

bool is_straight(const vector<card>&hand){
    int pips_v[5],i = 0;
    for(auto card_val:hand){
        pips_v[i++] = (card_val.get_pips().get_pips());
    }
    sort(pips_v,pips_v+5);
    if(pips_v[0]!=1){
        return(is_sequential(pips_v));
    }
    else{
        return((is_sequential(pips_v)) || is_sequential(pips_v,1));
    }
}

bool is_straight_flush(vector<card>&hand){
    return (is_flush(hand) && is_straight(hand));
}

int main(){
    vector<card>deck(52);
    init_deck(deck);
    srand(time(0));
    int how_many;
    int flush_count = 0;
    int str_count = 0;
    int str_flush_count = 0;
    cout<<"How Many shuffles?";
    cin>>how_many;
    for(int loop = 0;loop<how_many;loop++){
        random_shuffle(deck.begin(),deck.end());//stl algo
        vector<card>hand(5);
        int i = 0;
        for(auto p = deck.begin();i<5;++p){
            hand[i++] = *p;
        }
        if(is_flush(hand)){flush_count++;}
        if(is_straight(hand)){str_count++;}
        if(is_straight_flush(hand)){str_flush_count++;}
    }
    //print_deck(deck);
    cout<<"Flushes "<< flush_count <<" out of "<<how_many<<endl;
    cout<<"Straights "<< str_count <<" out of "<<how_many<<endl;
    cout<<"Straight Flushes "<< str_flush_count
    <<" out of "<<how_many<<endl;

}
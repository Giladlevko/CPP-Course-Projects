#include <iostream>
#include <random>
#include <ctime>
using namespace std;
const int sides = 6;
inline int r_sides(){return(rand()%sides + 1);}


int main(){
    const int n_dice = 2;
    int trials = 1;
    uniform_int_distribution<unsigned> u(1,6);
    default_random_engine e(time(0));

    cout << "\nEnter number of trials: ";
    cin >> trials;
    int *outcomes = new int[n_dice*sides+1]();
    for (int j = 0; j< trials;++j){
        int roll = 0;
        for (int k = 1; k <= n_dice; ++k){
            roll+=u(e);
        }
        outcomes[roll]++;
    }
    cout << "\nProbability:\n";
    for (int j = 1*n_dice; j < n_dice * sides + 1;++j){
        cout << "j = "<< j << " p = "<< static_cast<double>(outcomes[j])/trials << endl;
    }
    cout << "\n\n";
}
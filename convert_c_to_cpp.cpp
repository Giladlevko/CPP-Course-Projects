//Name: Mr.lad
//Date: 4/27/2026
//This program was originally written in C and converted to C++.
//(The original program in C is written below)
//Description:
// creates an array of integers from 0 to N-1, suses an inline
// function to calculate the sum of all elements, and prints the result.

//Original program in C:
//void sum(int*p, int n, int d[]){
    //int i;
    //*p = 0;
    // for(i = 0; i < n; ++i)
        //*p = *p + d[i];
//}
//int main(){
    //int i;
    //int accum = 0;
    //int data[N];
    //for(i = 0; i < N; ++i){
        //data[i] = i;
    //}
    //sum(&accum, N, data);
    //printf("sum is %d\n", accum); 
    //return 0;
//}

//Includes input/output tools
#include <iostream>
// Includes the vector type
#include <vector>
//Allows use of standard names without writing std::.
using namespace :: std;
//Defines a constant integer N with value 40.
const int N = 40;

//Name: sum
//Input: 
// int *p (pointer to store result), 
// int n (size of array), int d[] (integer array)
//Output: None
//Description: 
// Computes the sum of all elements in array d and stores the result in *p.
inline void sum(int *p, int n, const vector<int>& d){
    *p = 0;
    for(int i = 0; i < n; ++i){
        *p +=d[i];
    }
}

//Name: main
//Input: None
//Output: returns 0
//Description: 
// Initializes an array with values 0 to N-1, 
// uses the inline sum function to compute the total,
// and prints the result.
int main(void){
    vector<int> data(N);
    int accum = 0;
    for(int i = 0; i < N; ++i){
        data[i] = i;
    }
    sum(&accum,N,data);
    cout << "The sum is: " << accum << endl;
    return 0;
}

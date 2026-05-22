#include <iostream>
#include<string>
#include<thread>
using namespace std;
//use g++ -pthread  thread_example.cpp -o a when compiling
void message1(){
    cout<<"write message!\n";
}

int main(int argc,char**argv){
    //creates a process that can run concurrently
    thread foo([](){// thread as lambda expression
        cout<<"hello world\n";
    });
    thread thr1(message1);
    thr1.join();
    foo.join();// join synchronizes threads
}
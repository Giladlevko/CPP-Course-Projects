#include <iostream>
#include <iterator>
#include <fstream>
#include <algorithm>//used for sort
#include <vector>
using namespace std;

int main(){
    ifstream word_file("words.txt");
    //start and end are pointer types
    //in reading from the file the input operator for
    // strings uses white space so eacg white space
    // set of characters is read as one string
    istream_iterator<string> start(word_file),end;
    vector<string> words(start,end);
    
    cout<<"\n\nwords as read:\n";
    for (auto str: words){
        cout << str << "\t";
    }
    //sort needs two arguments of typr iterator to work
    //In this case the words will be sorted by their
    // first letter's ASCII value, if the value is the same
    // it'll cheack the next letter and so on
    //This sort routine expects that the elements pointed
    // at can be randomly accessed meaning being able 
    // to jump to an item directly
    sort(words.begin(),words.end());
    cout << "\n\nwords as sorted:\n";
    for (auto str : words){
        cout << str << "\t";
    }
    cout<<endl;

}
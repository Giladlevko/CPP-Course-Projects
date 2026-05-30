#include <iostream>
#include <vector>
using namespace std;

long long max_pairwise(vector<int>nums){
    int largest_num_1_index;
    long long largest_num_1 = -1; 
    for(int i = 0;i<nums.size();i++){
        if (nums[i]>largest_num_1){
            largest_num_1_index = i;
            largest_num_1 = nums[i];
        }

    }
    int largest_num_2_index;
    long long largest_num_2 = -1; 
    for(int j = 0;j<nums.size();j++){
        if ((nums[j]>largest_num_2)&&(j!=largest_num_1_index)){
            largest_num_2_index = j;
            largest_num_2 = nums[j];
        }

    }
    return (largest_num_1 * largest_num_2);

}

int main(){
    int n = 0;
    vector<int> numbers;
    //"Enter number amount (>=2): "
    cin>>n;
    //"Enter numbers: "
    for(int i = 0;i<n;i++){
        int number = 0;
        cin>>number;
        numbers.push_back(number);
    }
    //result
    cout<<max_pairwise(numbers);
}
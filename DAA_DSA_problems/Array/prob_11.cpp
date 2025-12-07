#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int maxSum = nums[0];       // stores the maximum sum found so far
        int currentSum = nums[0];   // stores the current subarray sum

        for (int i = 1; i < nums.size(); i++) {
            
            currentSum = max(nums[i], currentSum + nums[i]);
            
            // Update maximum sum
            maxSum = max(maxSum, currentSum);
        }
        return maxSum;
    }
};

int main() {
    vector<int> nums = {2, 3, 5, -2, 7, -4};

    Solution obj;
    cout << obj.maxSubArray(nums) << endl;  
    return 0;
}

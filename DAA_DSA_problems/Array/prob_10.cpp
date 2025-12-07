#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int candidate = 0, count = 0;

        // using Boyer-Moore
        for (int num : nums) {
            if (count == 0) {
                candidate = num;
            }
            count += (num == candidate) ? 1 : -1;
        }

        
        return candidate;
    }
};

int main() {
    vector<int> nums = {7, 0, 0, 1, 7, 7, 2, 7, 7};

    Solution obj;
    cout << obj.majorityElement(nums) << endl;  
    return 0;
}

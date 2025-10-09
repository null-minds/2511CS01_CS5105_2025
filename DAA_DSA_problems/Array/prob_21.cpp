#include <bits/stdc++.h>
using namespace std;

//Brute Force Approach

vector<int> twoSumBruteForce(vector<int> &nums, int target) {
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (nums[i] + nums[j] == target) {
                return {i, j};
            }
        }
    }
    return {}; // Not needed per constraints, but safe to include
}

//Optimal Approach (Using Hash Map)
   
vector<int> twoSumOptimal(vector<int> &nums, int target) {
    unordered_map<int, int> mp;
    for (int i = 0; i < nums.size(); i++) {
        int complement = target - nums[i];

        if (mp.find(complement) != mp.end()) {
            vector<int> res = {mp[complement], i};
            sort(res.begin(), res.end()); // ensure increasing order
            return res;
        }
        mp[nums[i]] = i;
    }
    return {};
}

// Main Function (Driver Code)
   
int main() {
    vector<int> nums = {2, 7, 11, 15};
    int target = 9;

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << "\nTarget: " << target << endl;

    vector<int> ans1 = twoSumBruteForce(nums, target);
    vector<int> ans2 = twoSumOptimal(nums, target);

    cout << "Brute Force Result: ";
    for (int x : ans1) cout << x << " ";
    cout << endl;

    cout << "Optimal Result: ";
    for (int x : ans2) cout << x << " ";
    cout << endl;

    return 0;
}

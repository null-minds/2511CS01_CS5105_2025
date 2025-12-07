#include <bits/stdc++.h>
using namespace std;


   //Brute Force Approach
   


int longestSubarrayBruteForce(vector<int> &nums, int k) {
    int n = nums.size();
    int maxLen = 0;

    for (int i = 0; i < n; i++) {
        int sum = 0;
        for (int j = i; j < n; j++) {
            sum += nums[j];
            if (sum == k) {
                maxLen = max(maxLen, j - i + 1);
            }
        }
    }
    return maxLen;
}

//Optimal Approach (Using Hash Map + Prefix Sum)


int longestSubarrayOptimal(vector<int> &nums, int k) {
    unordered_map<int, int> prefixSumIndex;
    int sum = 0, maxLen = 0;

    for (int i = 0; i < nums.size(); i++) {
        sum += nums[i];

        if (sum == k)
            maxLen = i + 1;

        if (prefixSumIndex.find(sum - k) != prefixSumIndex.end())
            maxLen = max(maxLen, i - prefixSumIndex[sum - k]);

        if (prefixSumIndex.find(sum) == prefixSumIndex.end())
            prefixSumIndex[sum] = i;
    }
    return maxLen;
}

/* ==================================================
   Main Function (Driver Code)
   ================================================== */
int main() {
    vector<int> nums = {10, 5, 2, 7, 1, 9};
    int k = 15;

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << "\nK = " << k << endl;

    cout << "Brute Force Result: " << longestSubarrayBruteForce(nums, k) << endl;
    cout << "Optimal Result: " << longestSubarrayOptimal(nums, k) << endl;

    return 0;
}

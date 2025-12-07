#include <bits/stdc++.h>
using namespace std;

// Brute Force Approach (O(n^2))
int countSubarraysBrute(vector<int>& nums, int k) {
    int n = nums.size();
    int count = 0;

    for (int i = 0; i < n; i++) {
        int sum = 0;
        for (int j = i; j < n; j++) {
            sum += nums[j];
            if (sum == k) count++;
        }
    }
    return count;
}

// Optimal Approach using Prefix Sum + HashMap (O(n))
int countSubarraysOptimal(vector<int>& nums, int k) {
    unordered_map<int, int> prefixFreq;
    prefixFreq[0] = 1; // base case for subarray starting at index 0

    int prefixSum = 0, count = 0;
    for (int num : nums) {
        prefixSum += num;

        // Check if prefixSum - k exists
        if (prefixFreq.find(prefixSum - k) != prefixFreq.end()) {
            count += prefixFreq[prefixSum - k];
        }

        // Store/update prefixSum frequency
        prefixFreq[prefixSum]++;
    }
    return count;
}

int main() {
    vector<int> nums1 = {1, 1, 1};
    int k1 = 2;
    cout << "Brute Force Result (Example1): " << countSubarraysBrute(nums1, k1) << endl;
    cout << "Optimal Result (Example1): " << countSubarraysOptimal(nums1, k1) << endl;

    vector<int> nums2 = {1, 2, 3};
    int k2 = 3;
    cout << "Brute Force Result (Example2): " << countSubarraysBrute(nums2, k2) << endl;
    cout << "Optimal Result (Example2): " << countSubarraysOptimal(nums2, k2) << endl;

    return 0;
}

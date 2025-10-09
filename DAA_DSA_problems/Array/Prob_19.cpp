#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

// Brute Force Approach: Check all possible subarrays
class BruteForceSolution {
public:
    int longestSubarrayWithSumK(vector<int>& nums, int k) {
        int maxLength = 0;
        int n = nums.size();
        // Iterate over all possible subarrays
        for (int i = 0; i < n; ++i) {
            long long sum = 0; // Use long long to avoid overflow
            for (int j = i; j < n; ++j) {
                sum += nums[j];
                // If sum equals k, update maxLength if current subarray is longer
                if (sum == k) {
                    maxLength = max(maxLength, j - i + 1);
                }
            }
        }
        return maxLength;
    }
};

// Optimal Approach: Use prefix sum with hash map
class OptimalSolution {
public:
    int longestSubarrayWithSumK(vector<int>& nums, int k) {
        unordered_map<long long, int> prefixSum; // Map to store prefix sum and earliest index
        long long sum = 0; // Current prefix sum
        int maxLength = 0;
        prefixSum[0] = -1; // Initialize for subarray starting at index 0
        // Iterate through the array
        for (int i = 0; i < nums.size(); ++i) {
            sum += nums[i];
            // If (sum - k) exists, a subarray with sum k is found
            if (prefixSum.find(sum - k) != prefixSum.end()) {
                maxLength = max(maxLength, i - prefixSum[sum - k]);
            }
            // Store the earliest index for this sum to maximize subarray length
            if (prefixSum.find(sum) == prefixSum.end()) {
                prefixSum[sum] = i;
            }
        }
        return maxLength;
    }
};

// Main function to handle input and output
int main() {
    int n, k;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    cin >> k;

    OptimalSolution opt;
    cout << opt.longestSubarrayWithSumK(nums, k) << endl;
    return 0;
}
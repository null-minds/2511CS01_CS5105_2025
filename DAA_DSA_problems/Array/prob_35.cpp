#include <bits/stdc++.h>
using namespace std;

int countSubarraysXor(vector<int>& nums, int k) {
    unordered_map<int, int> freq;
    int prefixXor = 0, count = 0;

    for (int num : nums) {
        prefixXor ^= num;

        // Case 1: subarray from start has XOR == k
        if (prefixXor == k) count++;

        // Case 2: check if there exists a prefix with XOR = prefixXor ^ k
        int need = prefixXor ^ k;
        if (freq.find(need) != freq.end()) {
            count += freq[need];
        }

        // Store current prefixXor
        freq[prefixXor]++;
    }
    return count;
}

int main() {
    vector<int> nums1 = {4, 2, 2, 6, 4};
    int k1 = 6;
    cout << "Output: " << countSubarraysXor(nums1, k1) << endl; // 4

    vector<int> nums2 = {5, 6, 7, 8, 9};
    int k2 = 5;
    cout << "Output: " << countSubarraysXor(nums2, k2) << endl; // 2

    return 0;
}


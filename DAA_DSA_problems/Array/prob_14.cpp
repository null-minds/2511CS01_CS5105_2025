#include <bits/stdc++.h>
using namespace std;

// Brute Force Approach (O(n^2))
int longestConsecutiveBrute(vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;

    int longest = 1;
    for (int i = 0; i < n; i++) {
        int current = nums[i];
        int count = 1;

        // Keep checking for next consecutive elements
        while (find(nums.begin(), nums.end(), current + 1) != nums.end()) {
            current += 1;
            count++;
        }
        longest = max(longest, count);
    }
    return longest;
}

// Optimal Approach using HashSet (O(n))
int longestConsecutiveOptimal(vector<int>& nums) {
    if (nums.empty()) return 0;

    unordered_set<int> s(nums.begin(), nums.end());
    int longest = 0;

    for (int num : s) {
        // Check if this is the start of a sequence
        if (s.find(num - 1) == s.end()) {
            int currentNum = num;
            int count = 1;

            while (s.find(currentNum + 1) != s.end()) {
                currentNum++;
                count++;
            }
            longest = max(longest, count);
        }
    }
    return longest;
}

int main() {
    vector<int> nums1 = {100, 4, 200, 1, 3, 2};
    vector<int> nums2 = {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};

    cout << "Brute Force Result (Example1): " << longestConsecutiveBrute(nums1) << endl;
    cout << "Optimal Result (Example1): " << longestConsecutiveOptimal(nums1) << endl;

    cout << "Brute Force Result (Example2): " << longestConsecutiveBrute(nums2) << endl;
    cout << "Optimal Result (Example2): " << longestConsecutiveOptimal(nums2) << endl;

    return 0;
}

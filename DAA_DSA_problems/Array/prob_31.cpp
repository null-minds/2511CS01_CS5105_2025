#include <bits/stdc++.h>
using namespace std;

// 🔹 Brute Force Approach (O(n^2))
int largestSubarraySumZeroBruteForce(vector<int>& arr) {
    int n = arr.size();
    int maxLen = 0;

    for (int i = 0; i < n; i++) {
        int sum = 0;
        for (int j = i; j < n; j++) {
            sum += arr[j];
            if (sum == 0) {
                maxLen = max(maxLen, j - i + 1);
            }
        }
    }

    return maxLen;
}

// 🔹 Optimal Approach (O(n)) using Hash Map
int largestSubarraySumZeroOptimal(vector<int>& arr) {
    unordered_map<int, int> prefixSumIndex; // stores first occurrence of prefix sum
    int sum = 0;
    int maxLen = 0;

    for (int i = 0; i < arr.size(); i++) {
        sum += arr[i];

        if (sum == 0) {
            maxLen = i + 1; // subarray from 0 to i
        } 
        else if (prefixSumIndex.find(sum) != prefixSumIndex.end()) {
            maxLen = max(maxLen, i - prefixSumIndex[sum]);
        } 
        else {
            prefixSumIndex[sum] = i; // store first occurrence
        }
    }

    return maxLen;
}

int main() {
    vector<int> arr = {15, -2, 2, -8, 1, 7, 10, 23};

    cout << "Brute Force Output: " << largestSubarraySumZeroBruteForce(arr) << endl;
    cout << "Optimal Output: " << largestSubarraySumZeroOptimal(arr) << endl;

    return 0;
}

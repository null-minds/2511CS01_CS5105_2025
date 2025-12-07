#include <bits/stdc++.h>
using namespace std;

// ------------------- Brute Force --------------------
int bruteHelper(int idx, int currSum, int K, vector<int>& arr) {
    if (idx == arr.size()) {
        return (currSum == K) ? 1 : 0;
    }

    // Option 1: pick element
    int pick = bruteHelper(idx + 1, currSum + arr[idx], K, arr);

    // Option 2: skip element
    int notPick = bruteHelper(idx + 1, currSum, K, arr);

    return pick + notPick;
}

int bruteForce(vector<int>& arr, int K) {
    return bruteHelper(0, 0, K, arr);
}


// ------------------- Optimal DP --------------------
int optimalDP(vector<int>& arr, int K) {
    int n = arr.size();
    vector<int> dp(K + 1, 0);

    dp[0] = 1; // One way to form sum 0 → take nothing

    for (int num : arr) {
        for (int t = K; t >= num; t--) {
            dp[t] += dp[t - num];
        }
    }

    return dp[K];
}


// ------------------- Main for testing --------------------
int main() {
    vector<int> arr = {1, 2, 3, 3};
    int K = 6;

    cout << "Brute Force Count: " << bruteForce(arr, K) << endl;
    cout << "Optimal DP Count : " << optimalDP(arr, K) << endl;

    return 0;
}


/*
 Time & Space Complexity

--- Brute Force Approach ---
Time Complexity   : O(2^N)
Space Complexity  : O(N)   (Recursion stack)

--- Optimal DP Approach ---
Time Complexity   : O(N * K)
Space Complexity  : O(K)   (1D DP array)

*/

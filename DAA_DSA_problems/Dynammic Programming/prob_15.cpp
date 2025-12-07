#include <bits/stdc++.h>
using namespace std;

// ------------------- Brute Force --------------------
int bruteHelper(int idx, int currSum, int totalSum, vector<int>& arr) {
    if (idx == arr.size()) {
        int otherSum = totalSum - currSum;
        return abs(currSum - otherSum);
    }

    // Option 1: Pick element in subset1
    int pick = bruteHelper(idx + 1, currSum + arr[idx], totalSum, arr);

    // Option 2: Do not pick element (goes to subset2)
    int notPick = bruteHelper(idx + 1, currSum, totalSum, arr);

    return min(pick, notPick);
}

int bruteForce(vector<int>& arr) {
    int totalSum = accumulate(arr.begin(), arr.end(), 0);
    return bruteHelper(0, 0, totalSum, arr);
}


// ------------------- Optimal DP --------------------
int optimalDP(vector<int>& arr) {
    int n = arr.size();
    int totalSum = accumulate(arr.begin(), arr.end(), 0);
    int target = totalSum / 2;

    vector<bool> dp(target + 1, false);
    dp[0] = true;

    for (int num : arr) {
        for (int t = target; t >= num; t--) {
            dp[t] = dp[t] || dp[t - num];
        }
    }

    int s1 = 0;
    for (int t = target; t >= 0; t--) {
        if (dp[t]) {
            s1 = t;
            break;
        }
    }

    return totalSum - 2 * s1;
}

// ------------------- Main (for testing) --------------------
int main() {
    vector<int> arr = {1, 6, 11, 5};

    cout << "Brute Force Result: " << bruteForce(arr) << endl;
    cout << "Optimal DP Result: " << optimalDP(arr) << endl;

    return 0;
}


/*
Time & Space Complexity

--- Brute Force Approach ---
Time Complexity   : O(2^N)
Space Complexity  : O(N)   (Recursion stack)

--- Optimal DP Approach ---
Time Complexity   : O(N * S)   where S = total sum of array
Space Complexity  : O(S)   (1D DP array)
*/
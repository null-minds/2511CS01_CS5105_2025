#include <bits/stdc++.h>
using namespace std;

int uniquePaths(int m, int n) {
    
    // Brute Force - Recursion
    // Time: O(2^(m+n)), Space: O(m+n)
    function<int(int,int)> brute = [&](int i, int j) -> int {
        if (i == m-1 && j == n-1) return 1;
        if (i >= m || j >= n) return 0;
        return brute(i+1, j) + brute(i, j+1);
    };
    
    // DP - Bottom-Up (Optimal)
    // Time: O(m*n), Space: O(n)  ← only one row needed
    vector<int> dp(n, 1);                     // first row all 1s
    for (int i = 1; i < m; i++) {             // start from row 1
        for (int j = 1; j < n; j++) {         // col 0 remains 1
            dp[j] += dp[j-1];                 // dp[j] = dp[j](above) + dp[j-1](left)
        }
    }
    return dp[n-1];
}

int main() {
    int m = 3, n = 7;
    cout << uniquePaths(m, n);  // Output: 28
    
    m = 3, n = 3;
    cout << "\n" << uniquePaths(m, n);  // Output: 6
}
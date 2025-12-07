#include <bits/stdc++.h>
using namespace std;

int n, m;
vector<vector<int>> grid;

// 1. BRUTE FORCE APPROACH - Recursion (Exponential Time)
// Tries all possible paths from (0,0) to (n-1,m-1)
// Time Complexity : O(2^(n+m))  → Very slow for large grids
// Space Complexity: O(n+m)      → Recursion stack

int minCostBrute(int i, int j) {
    // Base cases
    if (i == n - 1 && j == m - 1) {
        return grid[i][j];  // reached destination
    }
    if (i >= n || j >= m) {
        return INT_MAX;     // out of bounds → invalid path
    }

    // Two choices: go right or go down
    int goRight = minCostBrute(i, j + 1);
    int goDown  = minCostBrute(i + 1, j);

    // Take minimum of both paths and add current cell cost
    return grid[i][j] + min(goRight, goDown);
}

// 2. OPTIMAL APPROACH - Dynamic Programming (Bottom-Up)

// Fills dp table where dp[i][j] = min cost to reach (i,j) from (0,0)
// Time Complexity : O(n * m)
// Space Complexity: O(n * m)
// Can be further optimized to O(min(n,m)) space, but this is clear

int minCostDP() {
    vector<vector<int>> dp(n, vector<int>(m, 0));

    dp[0][0] = grid[0][0];

    for (int j = 1; j < m; j++) {
        dp[0][j] = dp[0][j-1] + grid[0][j];
    }

    for (int i = 1; i < n; i++) {
        dp[i][0] = dp[i-1][0] + grid[i][0];
    }

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            dp[i][j] = grid[i][j] + min(dp[i-1][j], dp[i][j-1]);
        }
    }

    return dp[n-1][m-1];
}

// Driver code

int main() {

    grid = {
        {1, 3, 1},
        {1, 5, 1},
        {4, 2, 1}
    };
    n = grid.size();
    m = grid[0].size();

    cout << "Grid:\n";
    for (auto& row : grid) {
        for (int x : row) cout << x << " ";
        cout << "\n";
    }

    cout << "\n=== Results ===\n";
    cout << "Brute Force (Recursion): " << minCostBrute(0, 0) << endl;
    // Time: Exponential → Don't run on large grids (>15x15)

    cout << "DP Optimal Approach   : " << minCostDP() << endl;
    // Time: O(n*m), Space: O(n*m)

    // Example 2 - Larger grid (Brute force will hang!)
    /*
    grid = vector<vector<int>>(20, vector<int>(20, 1));
    n = 20, m = 20;
    // minCostBrute(0,0) → will take forever
    cout << "Large Grid Min Cost (DP only): " << minCostDP() << endl;
    */

    return 0;
}
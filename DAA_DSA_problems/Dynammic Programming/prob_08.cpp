#include <bits/stdc++.h>
using namespace std;

int mazePaths(vector<vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    
    if (grid[0][0] == -1 || grid[n-1][m-1] == -1) return 0;
    
    // dp[i][j] = number of ways to reach (i,j)
    vector<vector<int>> dp(n, vector<int>(m, 0));
    dp[0][0] = 1;                                      // starting point
    
    // Fill first row
    for (int j = 1; j < m; j++) {
        if (grid[0][j] != -1) dp[0][j] = dp[0][j-1];
    }
    
    // Fill first column
    for (int i = 1; i < n; i++) {
        if (grid[i][0] != -1) dp[i][0] = dp[i-1][0];
    }
    
    // Fill rest of the grid
    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            if (grid[i][j] == -1) continue;           // blocked
            if (grid[i-1][j] != -1) dp[i][j] += dp[i-1][j];   // from top
            if (grid[i][j-1] != -1) dp[i][j] += dp[i][j-1];   // from left
        }
    }
    
    return dp[n-1][m-1];
}

// Space-optimized version (O(m) space)
int mazePathsOptimized(vector<vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    
    if (grid[0][0] == -1 || grid[n-1][m-1] == -1) return 0;
    
    vector<int> dp(m, 0);
    dp[0] = 1;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == -1) {
                dp[j] = 0;                         // block this cell
            } else if (j > 0) {
                dp[j] += dp[j-1];                  // add left
            }
        }
    }
    return dp[m-1];
}

int main() {
    vector<vector<int>> grid = {
        {0, 0, 0, 0},
        {0, -1, 0, 0},
        {0, 0, 0, -1},
        {0, 0, 0, 0}
    };
    
    cout << mazePaths(grid) << "\n";              // Output: 4
    cout << mazePathsOptimized(grid);             // Output: 4
}
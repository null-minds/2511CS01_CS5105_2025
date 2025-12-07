#include <bits/stdc++.h>
using namespace std;

// Solution class for minimum falling path sum using memoization
class Solution {
public:
    // Recursive helper function to find min path sum starting at (row, col)
    int dfs(int row, int col, vector<vector<int>>& matrix, vector<vector<int>>& dp) {
        // If column is out of bounds, return a large number to ignore this path
        if (col < 0 || col >= matrix[0].size()) {
            return 1e9;
        }

        // If we are at the last row, return the value at (row, col)
        if (row == matrix.size() - 1) {
            return matrix[row][col];
        }

        // If result is already computed, return from dp
        if (dp[row][col] != -1) {
            return dp[row][col];
        }

        // Recursively compute min falling path sum for the next row:
        // directly below, below-left, and below-right
        int down = dfs(row + 1, col, matrix, dp);
        int downLeft = dfs(row + 1, col - 1, matrix, dp);
        int downRight = dfs(row + 1, col + 1, matrix, dp);

        // Calculate minimum path sum for current cell
        int ans = matrix[row][col] + min({down, downLeft, downRight});

        // Store in dp and return
        return dp[row][col] = ans;
    }

    // Main function to find minimum falling path sum
    int minFallingPathSum(vector<vector<int>>& matrix) {
        int n = matrix.size();
        int m = matrix[0].size();

        // Initialize dp with -1 indicating uncomputed states
        vector<vector<int>> dp(n, vector<int>(m, -1));

        int minSum = 1e9;

        // Try starting from each element in the first row
        for (int col = 0; col < m; col++) {
            minSum = min(minSum, dfs(0, col, matrix, dp));
        }

        return minSum;
    }
};

int minFallingPathSum(vector<vector<int>>& matrix) {
        int n = matrix.size();
        int m = matrix[0].size();

        // Create dp array same size as matrix
        vector<vector<int>> dp(n, vector<int>(m, 0));

        // Initialize last row of dp same as matrix last row
        for (int col = 0; col < m; col++) {
            dp[n - 1][col] = matrix[n - 1][col];
        }

        // Iterate from second last row up to first row
        for (int row = n - 2; row >= 0; row--) {
            for (int col = 0; col < m; col++) {
                // Initialize minimum path sum as large number
                int down = dp[row + 1][col];

                int downLeft = (col > 0) ? dp[row + 1][col - 1] : 1e9;

                int downRight = (col < m - 1) ? dp[row + 1][col + 1] : 1e9;

                // Calculate minimum path sum for current cell
                dp[row][col] = matrix[row][col] + min({down, downLeft, downRight});
            }
        }

        // Find minimum among first row elements
        int minSum = *min_element(dp[0].begin(), dp[0].end());
        return minSum;
    }
};

int main() {
    vector<vector<int>> matrix = {
        {1, 4, 3, 1},
        {2, 3, -1, -1},
        {1, 1, -1, 8}
    };

    Solution sol;
    cout << "Minimum Falling Path Sum: " << sol.minFallingPathSum(matrix) << endl;

    return 0;
}
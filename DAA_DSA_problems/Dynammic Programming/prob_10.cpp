#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> triangle;
int n;

// 1. BRUTE FORCE APPROACH - Pure Recursion
// Tries all possible paths from top to bottom
// Time Complexity : O(2^n)   where n = number of rows
// Space Complexity: O(n)     recursion stack
int minPathBrute(int row, int col) {
    // Base case: reached last row
    if (row == n - 1) {
        return triangle[row][col];
    }

    int down      = minPathBrute(row + 1, col);         
    int downRight = minPathBrute(row + 1, col + 1);     

    return triangle[row][col] + min(down, downRight);
}

// 2. OPTIMAL APPROACH - Dynamic Programming (Bottom-Up)
// We fill from bottom to top → at each cell, we take min of two possible below cells
// Time Complexity : O(n²)   → total number of cells in triangle
// Space Complexity: O(n)    → only last row needed (optimized)
int minPathDP() {
    int rows = triangle.size();
    vector<int> dp = triangle[rows - 1];  // start with last row

    // Go from second-last row to top
    for (int row = rows - 2; row >= 0; row--) {
        for (int col = 0; col <= row; col++) {
            dp[col] = triangle[row][col] + min(dp[col], dp[col + 1]);
        }
        // After processing this row, we only keep first (row+1) elements active
        // But since vector size is fixed, we just overwrite safely
    }

    return dp[0]; 
}

// Driver code 
int main() {
    triangle = {
        {2},
        {3, 4},
        {6, 5, 7},
        {4, 1, 8, 3}
    };
    // Expected Answer: 11  (2 → 3 → 5 → 1)

    n = triangle.size();

    cout << "Triangle:\n";
    for (auto& row : triangle) {
        for (int x : row) cout << x << " ";
        cout << "\n";
    }
    cout << "\n";

    cout << "=== Results ===\n";
    cout << "Brute Force (Recursion): " << minPathBrute(0, 0) << endl;
    cout << "DP Optimal Approach   : " << minPathDP() << endl;

    // Test Case 2 - Single element
    cout << "\n--- Test Case 2 ---\n";
    triangle = {{5}};
    n = 1;
    cout << "Triangle: {5}\n";
    cout << "Brute Force: " << minPathBrute(0, 0) << endl;
    cout << "DP Optimal : " << minPathDP() << endl;

    return 0;
}
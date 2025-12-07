#include <iostream>
#include <vector>
#include <algorithm> // Required for std::min and std::abs
#include <climits>   // Required for INT_MAX

using namespace std;

int solveUtil(int ind, vector<int>& height, vector<int>& dp, int k) {
    // Base case: starting point (index 0) has zero cost
    if (ind == 0) return 0;

    // Return already computed result
    if (dp[ind] != -1) return dp[ind];

    // Initialize minimum steps as a large value
    int mmSteps = INT_MAX;

    // Try all possible jumps from 1 to k
    for (int j = 1; j <= k; j++) {
        // Ensure jump does not go out of bounds (ind - j must be >= 0)
        if (ind - j >= 0) {
            // Calculate the cost:
            // 1. Cost to reach previous stone (ind - j)
            // 2. Plus the cost of the current jump: |height[ind] - height[ind - j]|
            int jump_cost = abs(height[ind] - height[ind - j]);
            int prev_cost = solveUtil(ind - j, height, dp, k);

            int jump = prev_cost + jump_cost;
            
            // Store the minimum cost
            mmSteps = min(jump, mmSteps);
        }
    }
    
    // Save the result in dp array and return it
    return dp[ind] = mmSteps;
}

int solve(int n, vector<int>& height, int k) {
    if (n == 0) return 0;
    // DP array initialized to -1
    vector<int> dp(n, -1);
    // Start recursion from the last index (n-1)
    return solveUtil(n - 1, height, dp, k);
}

int main() {
    int n, k;

    // 1. Get the number of stones (n)
    cout << "Enter the number of stones (n): ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid input for the number of stones. Exiting." << endl;
        return 1;
    }
    
    // 2. Get the maximum jump size (k)
    cout << "Enter the maximum jump size (k): ";
    if (!(cin >> k) || k <= 0) {
        cout << "Invalid input for the maximum jump size. Exiting." << endl;
        return 1;
    }

    // 3. Get the heights
    vector<int> height(n);
    cout << "Enter the " << n << " stone heights separated by spaces: ";
    
    for (int i = 0; i < n; ++i) {
        if (!(cin >> height[i])) {
             cout << "\nInvalid input for stone height. Exiting." << endl;
             return 1;
        }
    }

    // Output the inputs for confirmation
    cout << "\n--- Input Summary ---" << endl;
    cout << "Heights: [";
    for (int i = 0; i < n; ++i) {
        cout << height[i] << (i == n - 1 ? "" : ", ");
    }
    cout << "]" << endl;
    cout << "Max Jump (k): " << k << endl;
    cout << "---------------------" << endl;


    // 4. Calculate and Output the minimum cost
    // The problem is "Frog Jump with K Steps," a classic Dynamic Programming problem. 
    int min_cost = solve(n, height, k);
    cout << "The minimum cost (energy) to reach the last stone is: **" << min_cost << "**" << endl;

    return 0;
}
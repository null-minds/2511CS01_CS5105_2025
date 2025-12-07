#include <iostream>
#include <vector>
#include <algorithm> // Required for std::max

using namespace std;

int solve_recursive(vector<int>& arr, int i, vector<int>& dp);
int maximumNonAdjacentSum(vector<int>& arr);

int solve_recursive(vector<int>& arr, int i, vector<int>& dp) {
    // Base case: If index is negative, no element to pick, sum is 0.
    if (i < 0) return 0;

    // Return already computed value
    if (dp[i] != -1) return dp[i];

    // Choice 1: Include current element arr[i]
    // Add arr[i] to the maximum sum from elements up to index i-2 (non-adjacent)
    int pick = arr[i] + solve_recursive(arr, i - 2, dp);

    // Choice 2: Exclude current element arr[i]
    // The maximum sum is the same as the maximum sum from elements up to index i-1
    int notPick = solve_recursive(arr, i - 1, dp);

    // Store the max of both choices in the DP table and return it
    return dp[i] = max(pick, notPick);
}

int maximumNonAdjacentSum(vector<int>& arr) {
    int n = arr.size();
    if (n == 0) return 0; // Handle empty array case

    // DP array initialized with -1 to indicate uncomputed states
    vector<int> dp(n, -1);

    // Start solving from the last index (n-1)
    return solve_recursive(arr, n - 1, dp);
}

// --- Main Execution Block ---
int main() {
    int n;
    cout << "Enter the number of elements in the array: ";
    // Read the size of the array
    if (!(cin >> n) || n < 0) {
        cout << "Invalid input for the number of elements." << endl;
        return 1;
    }

    // Handle trivial case
    if (n == 0) {
        cout << "The maximum non-adjacent sum is: 0" << endl;
        return 0;
    }

    vector<int> arr(n);
    cout << "Enter " << n << " elements separated by spaces: ";
    
    // Loop to read all elements into the vector
    for (int i = 0; i < n; ++i) {
        if (!(cin >> arr[i])) {
             cout << "\nInvalid input for array element." << endl;
             return 1;
        }
    }

    // Output the array for confirmation
    cout << "\nThe input array is: [";
    for (int i = 0; i < n; ++i) {
        cout << arr[i] << (i == n - 1 ? "" : ", ");
    }
    cout << "]" << endl;

    // Calculate the result using the function
    int maxSum = maximumNonAdjacentSum(arr);

    // Output the final result
    cout << "The maximum non-adjacent sum is: " << **maxSum** << endl;

    return 0;
}
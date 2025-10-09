#include <bits/stdc++.h>
using namespace std;

/* ===============================
   Brute Force Approach
   =============================== */

int maxProductBruteForce(vector<int> &nums) {
    int n = nums.size();
    int maxProduct = INT_MIN;

    for (int i = 0; i < n; i++) {
        int prod = 1;
        for (int j = i; j < n; j++) {
            prod *= nums[j];
            maxProduct = max(maxProduct, prod);
        }
    }
    return maxProduct;
}

/* ===============================
   Optimal Approach (Kadane’s for Product)
   =============================== */

int maxProductOptimal(vector<int> &nums) {
    int n = nums.size();
    int maxProd = nums[0];
    int minProd = nums[0];
    int ans = nums[0];

    for (int i = 1; i < n; i++) {
        int temp = maxProd;
        maxProd = max({nums[i], nums[i] * maxProd, nums[i] * minProd});
        minProd = min({nums[i], nums[i] * temp, nums[i] * minProd});
        ans = max(ans, maxProd);
    }
    return ans;
}

/* ===============================
   Main Function (Driver Code)
   =============================== */
int main() {
    vector<int> nums = {2, 3, -2, 4, -1}; // Example input

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << "\n";

    cout << "Brute Force Result: " << maxProductBruteForce(nums) << endl;
    cout << "Optimal Result: " << maxProductOptimal(nums) << endl;

    return 0;
}

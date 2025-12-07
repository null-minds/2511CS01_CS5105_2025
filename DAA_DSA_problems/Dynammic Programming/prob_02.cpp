

#include <bits/stdc++.h>
using namespace std;

// Brute-force: exponential recursion
int frogBrute(int idx, const vector<int> &h) {
    if (idx == 0) return 0;               // cost to stand at stair 0 is 0
    int oneStep = frogBrute(idx - 1, h) + abs(h[idx] - h[idx - 1]);
    int twoStep = INT_MAX;
    if (idx > 1) {
        twoStep = frogBrute(idx - 2, h) + abs(h[idx] - h[idx - 2]);
    }
    return min(oneStep, twoStep);
}

int frogOptimal(const vector<int> &h) {
    int n = h.size();
    if (n <= 1) return 0;                   // already at last stair

    int prev2 = 0;                          // dp[0]
    int prev1 = abs(h[1] - h[0]);           // dp[1]

    for (int i = 2; i < n; ++i) {
        int oneStep = prev1 + abs(h[i] - h[i - 1]);
        int twoStep = prev2 + abs(h[i] - h[i - 2]);
        int cur = min(oneStep, twoStep);    // dp[i]
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;                           // dp[n-1]
}

int main() {
    int n;
    cin >> n;
    vector<int> height(n);
    for (int i = 0; i < n; ++i) cin >> height[i];

    cout << "Bruteforce: "
         << frogBrute(n - 1, height) << "\n";
    cout << "Optimal: "
         << frogOptimal(height) << "\n";
    return 0;
}

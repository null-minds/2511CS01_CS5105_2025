#include <bits/stdc++.h>
using namespace std;

//Optmal DP
long long waysOptimal(int n) {
    if (n <= 0) return 0;   
    if (n == 1) return 1;

    long long prev2 = 1;    // ways to reach stair 0
    long long prev1 = 1;    // ways to reach stair 1

    for (int i = 2; i <= n; ++i) {
        long long cur = prev1 + prev2;   // ways[i] = ways[i-1] + ways[i-2]
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;           // ways to reach stair n
}

// Brute-force recursive
long long waysBruteForce(int n, int current = 0) {
    if (current == n) return 1;
    if (current > n)  return 0;
    return waysBruteForce(n, current + 1) +
           waysBruteForce(n, current + 2);
}

int main() {
    int n;
    cin >> n;

    cout << "Bruteforce: " << waysBruteForce(n) << "\n";
    cout << "Optimal: "    << waysOptimal(n)     << "\n";
    return 0;
}

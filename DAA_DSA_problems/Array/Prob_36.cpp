#include <bits/stdc++.h>
using namespace std;

// Brute Force: Count frequency using a map
class BruteForceSolution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        int n = nums.size();
        unordered_map<int,int> freq;
        for (int x : nums) freq[x]++;
        int rep = -1, miss = -1;
        for (int i = 1; i <= n; ++i) {
            if (freq[i] == 2) rep = i;
            else if (freq[i] == 0) miss = i;
        }
        return {rep, miss};
    }
};

// Optimal: Use mathematical relations (sum and sum of squares)
class OptimalSolution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        long long n = nums.size();
        long long sumN = n * (n + 1) / 2;
        long long sumSqN = n * (n + 1) * (2 * n + 1) / 6;
        long long sumA = 0, sumSqA = 0;
        for (long long x : nums) {
            sumA += x;
            sumSqA += (long long)x * x;
        }
        long long diff = sumA - sumN;                  // A - B
        long long diffSq = sumSqA - sumSqN;            // A^2 - B^2 = (A - B)(A + B)
        long long sumAB = diffSq / diff;               // A + B
        long long A = (diff + sumAB) / 2;
        long long B = sumAB - A;
        return {(int)A, (int)B};
    }
};

int main() {
    int n;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i) cin >> nums[i];

    OptimalSolution opt;
    vector<int> res = opt.findErrorNums(nums);
    cout << "[" << res[0] << "," << res[1] << "]" << endl;
    return 0;
}

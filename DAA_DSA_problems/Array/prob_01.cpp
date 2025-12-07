#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    // ---------- Brute Force: Iterative ----------
    int bruteForceMax(vector<int>& arr) {
        int maxElement = arr[0];
        for (int i = 1; i < arr.size(); i++) {
            if (arr[i] > maxElement) {
                maxElement = arr[i];
            }
        }
        return maxElement;
    }

    // ---------- Recursive Approach ----------
    int recursiveMax(vector<int>& arr, int n) {
        // Base case
        if (n == 1) return arr[0];

        // Recursive case: get max of first n-1 elements
        return max(arr[n - 1], recursiveMax(arr, n - 1));
    }
};

int main() {
    Solution sol;

    // Example 1
    vector<int> arr1 = {2, 5, 1, 3, 0};
    cout << "Brute Force Max (arr1): " << sol.bruteForceMax(arr1) << endl;
    cout << "Recursive Max (arr1): " << sol.recursiveMax(arr1, arr1.size()) << endl;

    // Example 2
    vector<int> arr2 = {8, 10, 5, 7, 9};
    cout << "Brute Force Max (arr2): " << sol.bruteForceMax(arr2) << endl;
    cout << "Recursive Max (arr2): " << sol.recursiveMax(arr2, arr2.size()) << endl;

    return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

class Solution {
public:
    // ---------- Brute Force: Sort ----------
    pair<int, int> bruteForce(vector<int>& arr) {
        int n = arr.size();
        sort(arr.begin(), arr.end());

        int smallest = arr[0], secondSmallest = -1;
        for (int i = 1; i < n; i++) {
            if (arr[i] > smallest) {
                secondSmallest = arr[i];
                break;
            }
        }

        int largest = arr[n - 1], secondLargest = -1;
        for (int i = n - 2; i >= 0; i--) {
            if (arr[i] < largest) {
                secondLargest = arr[i];
                break;
            }
        }

        return {secondSmallest, secondLargest};
    }

    // ---------- Optimal: Single Traversal ----------
    pair<int, int> optimal(vector<int>& arr) {
        int n = arr.size();
        if (n < 2) return {-1, -1};

        int smallest = INT_MAX, secondSmallest = INT_MAX;
        int largest = INT_MIN, secondLargest = INT_MIN;

        for (int num : arr) {
            // Update smallest & second smallest
            if (num < smallest) {
                secondSmallest = smallest;
                smallest = num;
            } else if (num < secondSmallest && num != smallest) {
                secondSmallest = num;
            }

            // Update largest & second largest
            if (num > largest) {
                secondLargest = largest;
                largest = num;
            } else if (num > secondLargest && num != largest) {
                secondLargest = num;
            }
        }

        if (secondSmallest == INT_MAX) secondSmallest = -1;
        if (secondLargest == INT_MIN) secondLargest = -1;

        return {secondSmallest, secondLargest};
    }
};

int main() {
    Solution sol;

    vector<int> arr = {1, 2, 4, 7, 7, 5};

    auto bruteAns = sol.bruteForce(arr);
    cout << "Brute Force -> Second Smallest: " << bruteAns.first
         << ", Second Largest: " << bruteAns.second << endl;

    auto optimalAns = sol.optimal(arr);
    cout << "Optimal -> Second Smallest: " << optimalAns.first
         << ", Second Largest: " << optimalAns.second << endl;

    return 0;
}

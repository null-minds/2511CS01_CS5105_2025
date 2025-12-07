#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Brute Force Approach: O(n^2) time complexity
bool isSortedBruteForce(const vector<int>& arr) {
    int n = arr.size();
    
    // Compare each element with all elements that come after it
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            // If any element is greater than a following element
            if (arr[i] > arr[j]) {
                return false;
            }
        }
    }
    return true;
}

// Optimal Approach: O(n) time complexity
bool isSortedOptimal(const vector<int>& arr) {
    int n = arr.size();
    
    // Check each adjacent pair
    for (int i = 0; i < n - 1; i++) {
        // If current element is greater than next element
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

// Alternative using STL is_sorted function
bool isSortedSTL(const vector<int>& arr) {
    return is_sorted(arr.begin(), arr.end());
}

int main() {
    // Test case from example
    vector<int> arr1 = {1, 2, 3, 4, 5};
    
    // Additional test cases
    vector<int> arr2 = {1, 2, 2, 3, 4}; // Equal consecutive elements (valid)
    vector<int> arr3 = {5, 4, 3, 2, 1}; // Decreasing order (invalid)
    vector<int> arr4 = {1, 3, 2, 4, 5}; // Not sorted (invalid)
    
    cout << "Test Case 1: {1,2,3,4,5}" << endl;
    cout << "Brute Force: " << (isSortedBruteForce(arr1) ? "True" : "False") << endl;
    cout << "Optimal: " << (isSortedOptimal(arr1) ? "True" : "False") << endl;
    cout << "STL: " << (isSortedSTL(arr1) ? "True" : "False") << endl;
    
    cout << "\nTest Case 2: {1,2,2,3,4}" << endl;
    cout << "Brute Force: " << (isSortedBruteForce(arr2) ? "True" : "False") << endl;
    cout << "Optimal: " << (isSortedOptimal(arr2) ? "True" : "False") << endl;
    
   
    return 0;
}

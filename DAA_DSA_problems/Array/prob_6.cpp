// Name: Samyaka
// Roll No.: 2511AI46

#include<bits/stdc++.h>
using namespace std;

// This function moves all zeros to the end of the vector.
// Time Complexity: O(n)
// Space Complexity: O(n)
void shiftZerosBruteForce(vector<int> &arr) {
    int n = arr.size();
    vector<int> nonZeroElements;

    // Collect all non-zero elements
    for (int i = 0; i < n; i++) {
        if (arr[i] != 0) {
            nonZeroElements.push_back(arr[i]);
        }
    }

    // Place non-zero elements at the beginning of the original vector
    for (int i = 0; i < nonZeroElements.size(); i++) {
        arr[i] = nonZeroElements[i];
    }

    // Fill the remaining spots with zeros
    for (int i = nonZeroElements.size(); i < n; i++) {
        arr[i] = 0;
    }
}

// This function provides a more efficient in-place solution.
// Time Complexity: O(n)
// Space Complexity: O(1)
void shiftZerosOptimal(vector<int> &arr) {
    int n = arr.size();
    int zeroIndex = -1;

    // Find the first zero element to act as a pivot.
    // We use this pointer to keep track of the first zero encountered.
    for (int i = 0; i < n; i++) {
        if (arr[i] == 0) {
            zeroIndex = i;
            break;
        }
    }

    // If there are no zeros, the array is already sorted.
    if (zeroIndex == -1) {
        return;
    }

    // Iterate through the array starting from the element after the first zero.
    // When a non-zero element is found, swap it with the element at the zeroIndex.
    for (int i = zeroIndex + 1; i < n; i++) {
        if (arr[i] != 0) {
            swap(arr[i], arr[zeroIndex]);
            zeroIndex++; // Move the zeroIndex to the next position
        }
    }
}

int main() {
    int size;
    cout << "Enter the size of the array: ";
    cin >> size;

    vector<int> data(size);
    cout << "Enter the elements: ";
    for (int i = 0; i < size; i++) {
        cin >> data[i];
    }

    // Uncomment one of the lines below to test a specific function.
    // shiftZerosBruteForce(data);
    shiftZerosOptimal(data);

    cout << "Array after moving zeros: ";
    for (int i = 0; i < size; i++) {
        cout << data[i] << " ";
    }
    cout << endl;

    return 0;
}

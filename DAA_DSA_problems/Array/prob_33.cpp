#include <bits/stdc++.h>
using namespace std;

void mergeArrays(vector<int>& nums1, int m, vector<int>& nums2, int n) {
    int i = m - 1;        // pointer for nums1's last valid element
    int j = n - 1;        // pointer for nums2's last element
    int k = m + n - 1;    // pointer for merged index in nums1

    // Merge from back
    while (i >= 0 && j >= 0) {
        if (nums1[i] > nums2[j]) {
            nums1[k--] = nums1[i--];
        } else {
            nums1[k--] = nums2[j--];
        }
    }

    // Copy remaining nums2 elements (if any)
    while (j >= 0) {
        nums1[k--] = nums2[j--];
    }
}

int main() {
    vector<int> nums1 = {-5, -2, 4, 5, 0, 0, 0}; // m+n size with extra space
    vector<int> nums2 = {-3, 1, 8};
    int m = 4, n = 3;

    mergeArrays(nums1, m, nums2, n);

    cout << "Merged Array: ";
    for (int x : nums1) cout << x << " ";
    cout << endl;

    return 0;
}


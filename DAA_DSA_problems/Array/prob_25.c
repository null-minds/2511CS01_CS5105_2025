#include <bits/stdc++.h>
using namespace std;

void nextPermutation(vector<int> &nums) {
    int n = nums.size();
    int bp = -1; // breakpoint index

    // Step 1: Find the first index from right such that nums[i] < nums[i+1]
    for (int i = n - 2; i >= 0; i--) {
        if (nums[i] < nums[i + 1]) {
            bp = i;
            break;
        }
    }

    // Step 2: If such index exists, find element just greater than nums[bp] from right and swap
    if (bp != -1) {
        for (int j = n - 1; j > bp; j--) {
            if (nums[j] > nums[bp]) {
                swap(nums[j], nums[bp]);
                break;
            }
        }
    }

    // Step 3: Reverse the suffix starting from bp+1 till end
    reverse(nums.begin() + bp + 1, nums.end());
}

int main() {
    int n;
    cout << "Enter size of array: ";
    cin >> n;

    vector<int> nums(n);
    cout << "Enter elements of array: ";
    for (int i = 0; i < n; i++) cin >> nums[i];

    nextPermutation(nums);

    cout << "Next Permutation: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    return 0;
}

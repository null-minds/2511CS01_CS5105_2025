#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> fourSum(vector<int>& nums, int target) {
    vector<vector<int>> res;
    int n = nums.size();
    if (n < 4) return res;

    sort(nums.begin(), nums.end());

    for (int i = 0; i < n - 3; i++) {
        if (i > 0 && nums[i] == nums[i - 1]) continue; // skip duplicate i

        for (int j = i + 1; j < n - 2; j++) {
            if (j > i + 1 && nums[j] == nums[j - 1]) continue; // skip duplicate j

            long long newTarget = (long long)target - nums[i] - nums[j];
            int left = j + 1, right = n - 1;

            while (left < right) {
                long long sum = nums[left] + nums[right];
                if (sum == newTarget) {
                    res.push_back({nums[i], nums[j], nums[left], nums[right]});
                    left++;
                    right--;

                    while (left < right && nums[left] == nums[left - 1]) left++;   // skip duplicate left
                    while (left < right && nums[right] == nums[right + 1]) right--; // skip duplicate right
                }
                else if (sum < newTarget) left++;
                else right--;
            }
        }
    }
    return res;
}

int main() {
    vector<int> nums = {1, -2, 3, 5, 7, 9};
    int target = 7;

    vector<vector<int>> ans = fourSum(nums, target);

    cout << "Quadruplets: " << endl;
    for (auto &q : ans) {
        cout << "[ ";
        for (int x : q) cout << x << " ";
        cout << "]" << endl;
    }
    return 0;
}


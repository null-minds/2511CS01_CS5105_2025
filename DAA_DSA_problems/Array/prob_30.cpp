#include <bits/stdc++.h>
using namespace std;

// 🔹 Brute Force Approach (O(n³))
vector<vector<int>> threeSumBruteForce(vector<int>& nums) {
    int n = nums.size();
    set<vector<int>> uniqueTriplets;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = j + 1; k < n; k++) {
                if (nums[i] + nums[j] + nums[k] == 0) {
                    vector<int> triplet = {nums[i], nums[j], nums[k]};
                    sort(triplet.begin(), triplet.end());  // avoid duplicates
                    uniqueTriplets.insert(triplet);
                }
            }
        }
    }

    // Convert set to vector for output
    vector<vector<int>> result(uniqueTriplets.begin(), uniqueTriplets.end());
    return result;
}

// 🔹 Optimal Approach (O(n²))
vector<vector<int>> threeSumOptimal(vector<int>& nums) {
    vector<vector<int>> res;
    sort(nums.begin(), nums.end());
    int n = nums.size();

    for (int i = 0; i < n - 2; i++) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;  // skip duplicates for i

        int left = i + 1;
        int right = n - 1;

        while (left < right) {
            int sum = nums[i] + nums[left] + nums[right];

            if (sum == 0) {
                res.push_back({nums[i], nums[left], nums[right]});

                // Skip duplicates for left and right
                while (left < right && nums[left] == nums[left + 1]) left++;
                while (left < right && nums[right] == nums[right - 1]) right--;

                left++;
                right--;
            } 
            else if (sum < 0)
                left++;
            else
                right--;
        }
    }

    return res;
}

int main() {
    vector<int> nums = {2, -2, 0, 3, -3, 5};

    cout << "🔸 Brute Force Output:\n";
    vector<vector<int>> brute = threeSumBruteForce(nums);
    for (auto& t : brute) {
        cout << "[ ";
        for (int x : t) cout << x << " ";
        cout << "]\n";
    }

    cout << "\n🔹 Optimal Output:\n";
    vector<vector<int>> optimal = threeSumOptimal(nums);
    for (auto& t : optimal) {
        cout << "[ ";
        for (int x : t) cout << x << " ";
        cout << "]\n";
    }

    return 0;
}

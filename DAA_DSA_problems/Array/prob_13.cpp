#include <bits/stdc++.h>
using namespace std;

// Brute Force Approach
vector<int> leadersBruteForce(vector<int>& nums) {
    int n = nums.size();
    vector<int> leaders;
    for (int i = 0; i < n; i++) {
        bool isLeader = true;
        for (int j = i + 1; j < n; j++) {
            if (nums[j] >= nums[i]) {
                isLeader = false;
                break;
            }
        }
        if (isLeader) {
            leaders.push_back(nums[i]);
        }
    }
    return leaders;
}

// Optimal Approach (O(n))
vector<int> leadersOptimal(vector<int>& nums) {
    int n = nums.size();
    vector<int> leaders;
    int maxRight = nums[n - 1];
    leaders.push_back(maxRight);
    
    for (int i = n - 2; i >= 0; i--) {
        if (nums[i] > maxRight) {
            leaders.push_back(nums[i]);
            maxRight = nums[i];
        }
    }
    reverse(leaders.begin(), leaders.end());
    return leaders;
}

int main() {
    vector<int> nums1 = {1, 2, 5, 3, 1, 2};
    vector<int> nums2 = {-3, 4, 5, 1, -4, -5};
    
    // Brute Force
    auto ans1 = leadersBruteForce(nums1);
    cout << "Brute Force Output 1: ";
    for (int x : ans1) cout << x << " ";
    cout << endl;
    
    auto ans2 = leadersBruteForce(nums2);
    cout << "Brute Force Output 2: ";
    for (int x : ans2) cout << x << " ";
    cout << endl;

    // Optimal
    auto ans3 = leadersOptimal(nums1);
    cout << "Optimal Output 1: ";
    for (int x : ans3) cout << x << " ";
    cout << endl;
    
    auto ans4 = leadersOptimal(nums2);
    cout << "Optimal Output 2: ";
    for (int x : ans4) cout << x << " ";
    cout << endl;

    return 0;
}

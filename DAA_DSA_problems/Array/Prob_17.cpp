#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

// Brute Force Approach: Combine, sort, and remove duplicates
class BruteForceSolution {
public:
    vector<int> findUnion(vector<int>& nums1, vector<int>& nums2) {
        // Combine both arrays
        vector<int> result;
        result.reserve(nums1.size() + nums2.size());
        result.insert(result.end(), nums1.begin(), nums1.end());
        result.insert(result.end(), nums2.begin(), nums2.end());
        
        // Sort the combined array
        sort(result.begin(), result.end());
        
        // Remove duplicates
        vector<int> unionArray;
        if (result.empty()) return unionArray;
        unionArray.push_back(result[0]);
        for (size_t i = 1; i < result.size(); ++i) {
            if (result[i] != result[i-1]) {
                unionArray.push_back(result[i]);
            }
        }
        return unionArray;
    }
};

// Optimal Approach: Two-pointer technique to merge and remove duplicates
class OptimalSolution {
public:
    vector<int> findUnion(vector<int>& nums1, vector<int>& nums2) {
        vector<int> unionArray;
        int i = 0, j = 0;
        // Two-pointer merge
        while (i < nums1.size() && j < nums2.size()) {
            // Skip duplicates in nums1
            while (i + 1 < nums1.size() && nums1[i] == nums1[i+1]) i++;
            // Skip duplicates in nums2
            while (j + 1 < nums2.size() && nums2[j] == nums2[j+1]) j++;
            
            if (nums1[i] < nums2[j]) {
                // Add nums1[i] if it's different from last added
                if (unionArray.empty() || unionArray.back() != nums1[i]) {
                    unionArray.push_back(nums1[i]);
                }
                i++;
            } else if (nums1[i] > nums2[j]) {
                // Add nums2[j] if it's different from last added
                if (unionArray.empty() || unionArray.back() != nums2[j]) {
                    unionArray.push_back(nums2[j]);
                }
                j++;
            } else {
                // Equal elements, add one and advance both pointers
                if (unionArray.empty() || unionArray.back() != nums1[i]) {
                    unionArray.push_back(nums1[i]);
                }
                i++;
                j++;
            }
        }
        // Process remaining elements in nums1
        while (i < nums1.size()) {
            if (unionArray.empty() || unionArray.back() != nums1[i]) {
                unionArray.push_back(nums1[i]);
            }
            i++;
        }
        // Process remaining elements in nums2
        while (j < nums2.size()) {
            if (unionArray.empty() || unionArray.back() != nums2[j]) {
                unionArray.push_back(nums2[j]);
            }
            j++;
        }
        return unionArray;
    }
};

// Main function to handle input and output
int main() {
    int n, m;
    cin >> n;
    vector<int> nums1(n);
    for (int i = 0; i < n; ++i) {
        cin >> nums1[i];
    }
    cin >> m;
    vector<int> nums2(m);
    for (int i = 0; i < m; ++i) {
        cin >> nums2[i];
    }

    OptimalSolution opt;
    vector<int> unionArray = opt.findUnion(nums1, nums2);
    // Output the union array
    for (int num : unionArray) {
        cout << num << " ";
    }
    cout << endl;
    return 0;
}
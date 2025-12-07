#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

// Brute Force Approach: Use hash map to count frequencies
class BruteForceSolution {
public:
    int singleNumber(vector<int>& nums) {
        unordered_map<int, int> freq;
        // Count frequency of each number
        for (int num : nums) {
            freq[num]++;
        }
        // Find the number with frequency 1
        for (const auto& pair : freq) {
            if (pair.second == 1) {
                return pair.first;
            }
        }
        return -1; // Should not reach here given problem constraints
    }
};

// Optimal Approach: Use XOR operation
class OptimalSolution {
public:
    int singleNumber(vector<int>& nums) {
        int result = 0;
        // XOR all numbers
        for (int num : nums) {
            result ^= num;
        }
        return result;
    }
};

// Main function to handle input and output
int main() {
    int n;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }

    OptimalSolution opt;
    cout << opt.singleNumber(nums) << endl;
    return 0;
}
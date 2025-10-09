#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <iostream>

using namespace std;

// Function to check if a given order is valid for the dictionary
bool isValidOrder(const vector<char>& order, const vector<string>& dict) {
    // Create a map to store character positions in the order
    vector<int> pos(26, -1);
    for (int i = 0; i < order.size(); ++i) {
        pos[order[i] - 'a'] = i;
    }
    // Check each consecutive pair of words
    for (int i = 0; i < dict.size() - 1; ++i) {
        const string& w1 = dict[i];
        const string& w2 = dict[i + 1];
        bool valid = false;
        // Compare characters of both words
        for (int j = 0; j < w1.size() && j < w2.size(); ++j) {
            if (w1[j] != w2[j]) {
                // Characters differ: w1[j] must come before w2[j] in order
                if (pos[w1[j] - 'a'] >= pos[w2[j] - 'a'] || pos[w1[j] - 'a'] == -1 || pos[w2[j] - 'a'] == -1) {
                    return false;
                }
                valid = true;
                break;
            }
        }
        // If w1 is a prefix of w2 and w1 is longer, it's invalid
        if (!valid && w1.size() > w2.size()) {
            return false;
        }
    }
    return true;
}

// Brute Force Approach: Try all permutations of characters
class BruteForceSolution {
public:
    vector<char> alienOrder(int K, vector<string>& dict) {
        vector<char> chars;
        // Collect all unique characters (up to K)
        for (const auto& word : dict) {
            for (char c : word) {
                if (chars.size() < K && find(chars.begin(), chars.end(), c) == chars.end()) {
                    chars.push_back(c);
                }
            }
        }
        // Try all permutations
        sort(chars.begin(), chars.end());
        do {
            if (isValidOrder(chars, dict)) {
                return chars; // Return the first valid order
            }
        } while (next_permutation(chars.begin(), chars.end()));
        return {}; // No valid order found
    }
};

// Optimal Approach: Topological Sort
class OptimalSolution {
private:
    // DFS to detect cycle and build topological order
    bool dfs(char c, vector<vector<int>>& adj, vector<int>& state, vector<char>& order) {
        int idx = c - 'a';
        if (state[idx] == 1) return false; // Cycle detected
        if (state[idx] == 2) return true;  // Already processed
        state[idx] = 1; // Mark as visiting
        // Visit all neighbors (characters that must come after c)
        for (int neighbor : adj[idx]) {
            if (!dfs(char('a' + neighbor), adj, state, order)) {
                return false; // Cycle found
            }
        }
        state[idx] = 2; // Mark as visited
        order.push_back(c); // Add to topological order
        return true;
    }

public:
    vector<char> alienOrder(int K, vector<string>& dict) {
        // Build adjacency list
        vector<vector<int>> adj(26);
        vector<bool> present(26, false);
        // Compare consecutive words to build graph
        for (int i = 0; i < dict.size() - 1; ++i) {
            const string& w1 = dict[i];
            const string& w2 = dict[i + 1];
            // Mark characters as present
            for (char c : w1) present[c - 'a'] = true;
            for (char c : w2) present[c - 'a'] = true;
            // Find first differing character
            for (int j = 0; j < w1.size() && j < w2.size(); ++j) {
                if (w1[j] != w2[j]) {
                    adj[w1[j] - 'a'].push_back(w2[j] - 'a'); // w1[j] -> w2[j]
                    break;
                }
            }
        }
        // Collect all characters (up to K)
        vector<char> chars;
        for (int i = 0; i < 26 && chars.size() < K; ++i) {
            if (present[i]) chars.push_back('a' + i);
        }
        // Perform topological sort using DFS
        vector<int> state(26, 0); // 0: unvisited, 1: visiting, 2: visited
        vector<char> order;
        for (char c : chars) {
            if (state[c - 'a'] == 0) {
                if (!dfs(c, adj, state, order)) {
                    return {}; // Cycle detected, no valid order
                }
            }
        }
        reverse(order.begin(), order.end()); // Reverse to get correct topological order
        return order;
    }
};

// Main function to handle input and output
int main() {
    int N, K;
    cin >> N >> K;
    vector<string> dict(N);
    for (int i = 0; i < N; ++i) {
        cin >> dict[i];
    }

    OptimalSolution opt;
    vector<char> order = opt.alienOrder(K, dict);
    // Output the order
    for (char c : order) {
        cout << c << " ";
    }
    cout << endl;
    return 0;
}
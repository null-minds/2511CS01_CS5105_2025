#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>

using namespace std;

// Function to check if a given order is valid for the prerequisites
bool isValidOrder(const vector<int>& order, const vector<vector<int>>& prerequisites) {
    // Create a map to store task positions in the order
    vector<int> pos(order.size());
    for (int i = 0; i < order.size(); ++i) {
        pos[order[i]] = i;
    }
    // Check if each prerequisite pair [a, b] has b before a in the order
    for (const auto& pre : prerequisites) {
        int task = pre[0], prereq = pre[1];
        if (pos[task] <= pos[prereq]) {
            return false; // Invalid order: prerequisite must come before task
        }
    }
    return true;
}

// Brute Force Approach for Problem I and II
class BruteForceSolution {
public:
    // Problem II: Check if all tasks can be completed
    bool canFinish(int numTasks, vector<vector<int>>& prerequisites) {
        vector<int> tasks(numTasks);
        for (int i = 0; i < numTasks; ++i) {
            tasks[i] = i;
        }
        // Try all permutations
        do {
            if (isValidOrder(tasks, prerequisites)) {
                return true; // Found a valid order
            }
        } while (next_permutation(tasks.begin(), tasks.end()));
        return false; // No valid order found
    }

    // Problem I: Find a valid order of tasks
    vector<int> findOrder(int numTasks, vector<vector<int>>& prerequisites) {
        vector<int> tasks(numTasks);
        for (int i = 0; i < numTasks; ++i) {
            tasks[i] = i;
        }
        // Try all permutations
        do {
            if (isValidOrder(tasks, prerequisites)) {
                return tasks; // Return the first valid order
            }
        } while (next_permutation(tasks.begin(), tasks.end()));
        return {}; // Return empty vector if no valid order
    }
};

// Optimal Approach for Problem I and II using Topological Sort (DFS)
class OptimalSolution {
private:
    // DFS to detect cycle and build topological order
    bool dfs(int task, vector<vector<int>>& adj, vector<int>& state, vector<int>& order) {
        // state: 0 = unvisited, 1 = visiting, 2 = visited
        if (state[task] == 1) return false; // Cycle detected
        if (state[task] == 2) return true;  // Already processed
        state[task] = 1; // Mark as visiting
        // Visit all prerequisites (neighbors)
        for (int prereq : adj[task]) {
            if (!dfs(prereq, adj, state, order)) {
                return false; // Cycle found
            }
        }
        state[task] = 2; // Mark as visited
        order.push_back(task); // Add to topological order
        return true;
    }

public:
    // Problem II: Check if all tasks can be completed
    bool canFinish(int numTasks, vector<vector<int>>& prerequisites) {
        // Build adjacency list (reverse graph: prereq -> task)
        vector<vector<int>> adj(numTasks);
        for (const auto& pre : prerequisites) {
            adj[pre[1]].push_back(pre[0]); // prereq -> task
        }
        vector<int> state(numTasks, 0); // 0: unvisited, 1: visiting, 2: visited
        vector<int> order;
        // Run DFS from each unvisited task
        for (int i = 0; i < numTasks; ++i) {
            if (state[i] == 0) {
                if (!dfs(i, adj, state, order)) {
                    return false; // Cycle detected
                }
            }
        }
        return true;
    }

    // Problem I: Find a valid order of tasks
    vector<int> findOrder(int numTasks, vector<vector<int>>& prerequisites) {
        // Build adjacency list (reverse graph: prereq -> task)
        vector<vector<int>> adj(numTasks);
        for (const auto& pre : prerequisites) {
            adj[pre[1]].push_back(pre[0]); // prereq -> task
        }
        vector<int> state(numTasks, 0); // 0: unvisited, 1: visiting, 2: visited
        vector<int> order;
        // Run DFS from each unvisited task
        for (int i = 0; i < numTasks; ++i) {
            if (state[i] == 0) {
                if (!dfs(i, adj, state, order)) {
                    return {}; // Cycle detected, return empty
                }
            }
        }
        reverse(order.begin(), order.end()); // Reverse to get correct topological order
        return order;
    }
};

// Main function to handle input and output
int main() {
    int numTasks, numPairs;
    cin >> numTasks >> numPairs;
    vector<vector<int>> prerequisites(numPairs, vector<int>(2));
    for (int i = 0; i < numPairs; ++i) {
        cin >> prerequisites[i][0] >> prerequisites[i][1];
    }

    OptimalSolution opt; // Using optimal solution for output
    // Problem II: Output "Yes" or "No"
    cout << (opt.canFinish(numTasks, prerequisites) ? "Yes" : "No") << endl;

    // Problem I: Output the order (or nothing if empty)
    vector<int> order = opt.findOrder(numTasks, prerequisites);
    // As per instruction, do not print the order
    return 0;
}
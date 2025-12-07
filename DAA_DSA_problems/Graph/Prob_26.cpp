#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>

using namespace std;

// Brute Force Approach: Check all paths from each node
class BruteForceSolution {
private:
    // DFS to check if all paths from node lead to a terminal node
    bool isSafeDFS(int node, const vector<vector<int>>& adj, vector<bool>& visited, unordered_set<int>& path) {
        // If node has no outgoing edges, it's a terminal node (safe)
        if (adj[node].empty()) return true;
        // If node is in current path, there's a cycle, so not safe
        if (path.count(node)) return false;
        path.insert(node);
        visited[node] = true;
        // Check all neighbors
        for (int neighbor : adj[node]) {
            if (!isSafeDFS(neighbor, adj, visited, path)) {
                path.erase(node);
                return false; // One path doesn't lead to terminal
            }
        }
        path.erase(node);
        return true;
    }

public:
    vector<int> eventualSafeNodes(int V, vector<vector<int>>& adj) {
        vector<int> result;
        vector<bool> visited(V, false);
        unordered_set<int> path;
        // Check each node
        for (int i = 0; i < V; ++i) {
            path.clear();
            if (isSafeDFS(i, adj, visited, path)) {
                result.push_back(i);
            }
        }
        sort(result.begin(), result.end()); // Sort in ascending order
        return result;
    }
};

// Optimal Approach: Use reverse graph and DFS
class OptimalSolution {
private:
    // DFS to mark nodes that can reach terminal nodes
    bool dfs(int node, const vector<vector<int>>& adj, vector<int>& state, vector<bool>& safe) {
        // state: 0 = unvisited, 1 = visiting, 2 = visited
        if (state[node] == 1) return false; // Cycle detected
        if (state[node] == 2) return safe[node]; // Already processed
        state[node] = 1; // Mark as visiting
        // If terminal node (no outgoing edges), mark as safe
        if (adj[node].empty()) {
            safe[node] = true;
            state[node] = 2;
            return true;
        }
        // Check all neighbors
        for (int neighbor : adj[node]) {
            if (!dfs(neighbor, adj, state, safe)) {
                state[node] = 2;
                return false; // Path leads to cycle or unsafe node
            }
        }
        safe[node] = true; // All paths lead to safe nodes
        state[node] = 2; // Mark as visited
        return true;
    }

public:
    vector<int> eventualSafeNodes(int V, vector<vector<int>>& adj) {
        vector<int> state(V, 0); // 0: unvisited, 1: visiting, 2: visited
        vector<bool> safe(V, false);
        vector<int> result;
        // Run DFS from each node
        for (int i = 0; i < V; ++i) {
            if (state[i] == 0) {
                dfs(i, adj, state, safe);
            }
            if (safe[i]) {
                result.push_back(i);
            }
        }
        sort(result.begin(), result.end()); // Sort in ascending order
        return result;
    }
};

// Main function to handle input and output
int main() {
    int V, E;
    cin >> V >> E;
    vector<vector<int>> adj(V);
    // Read edges
    for (int i = 0; i < E; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v); // Directed edge u -> v
    }

    OptimalSolution opt;
    vector<int> safeNodes = opt.eventualSafeNodes(V, adj);
    // Output safe nodes in sorted order
    for (int node : safeNodes) {
        cout << node << " ";
    }
    cout << endl;
    return 0;
}
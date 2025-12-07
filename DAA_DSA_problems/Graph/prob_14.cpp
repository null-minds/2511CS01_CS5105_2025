#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Brute-force DFS traversal to check for cycles.
// Time Complexity: O(V + E)
// Space Complexity: O(V)
bool detectCycleBruteForceUtil(int u, int parent, const vector<vector<int>>& adjMatrix, vector<bool>& visited) {
    visited[u] = true;
    int V = adjMatrix.size();

    for (int v = 0; v < V; ++v) {
        if (adjMatrix[u][v] == 1) { // Check for an edge in the adjacency matrix
            if (!visited[v]) {
                if (detectCycleBruteForceUtil(v, u, adjMatrix, visited)) {
                    return true;
                }
            } else if (v != parent) {
                return true;
            }
        }
    }
    return false;
}

bool isCycleBruteForce(int V, const vector<vector<int>>& adjMatrix) {
    if (V == 0) return false;
    vector<bool> visited(V, false);
    for (int i = 0; i < V; ++i) {
        if (!visited[i]) {
            if (detectCycleBruteForceUtil(i, -1, adjMatrix, visited)) {
                return true;
            }
        }
    }
    return false;
}

// Optimal BFS traversal to check for cycles.
// Time Complexity: O(V + E)
// Space Complexity: O(V)
bool detectCycleOptimalUtil(int start, const vector<vector<int>>& adj, vector<bool>& visited) {
    queue<pair<int, int>> q;
    q.push({start, -1});
    visited[start] = true;

    while (!q.empty()) {
        int node = q.front().first;
        int parent = q.front().second;
        q.pop();

        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push({neighbor, node});
            } else if (neighbor != parent) {
                return true;
            }
        }
    }
    return false;
}

bool isCycleOptimal(int V, const vector<vector<int>>& adj) {
    if (V == 0) return false;
    vector<bool> visited(V, false);
    for (int i = 0; i < V; ++i) {
        if (!visited[i]) {
            if (detectCycleOptimalUtil(i, adj, visited)) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    // Example 1: Graph with a cycle (0-1-2-0)
    int V1 = 3;
    vector<vector<int>> adj1 = {{1, 2}, {0}, {0}};
    vector<vector<int>> adjMatrix1 = {{0, 1, 1}, {1, 0, 0}, {1, 0, 0}};

    cout << "Example 1:" << endl;
    cout << "Optimal (Adjacency List): " << (isCycleOptimal(V1, adj1) ? "Cycle detected." : "No cycle detected.") << endl;
    cout << "Brute-Force (Adjacency Matrix): " << (isCycleBruteForce(V1, adjMatrix1) ? "Cycle detected." : "No cycle detected.") << endl;

    // Example 2: Graph without a cycle (a tree structure)
    int V2 = 4;
    vector<vector<int>> adj2 = {{1}, {0, 2}, {1, 3}, {2}};
    vector<vector<int>> adjMatrix2 = {{0, 1, 0, 0}, {1, 0, 1, 0}, {0, 1, 0, 1}, {0, 0, 1, 0}};

    cout << "\nExample 2:" << endl;
    cout << "Optimal (Adjacency List): " << (isCycleOptimal(V2, adj2) ? "Cycle detected." : "No cycle detected.") << endl;
    cout << "Brute-Force (Adjacency Matrix): " << (isCycleBruteForce(V2, adjMatrix2) ? "Cycle detected." : "No cycle detected.") << endl;

    return 0;
}

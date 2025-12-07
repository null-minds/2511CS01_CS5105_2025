#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class Solution {
public:
    // ---------- Brute Force: Recursive DFS -------------
    void dfsRecursive(int node, vector<vector<int>>& adj, vector<bool>& visited, vector<int>& result) {
        visited[node] = true;
        result.push_back(node + 1); // +1 for 1-based node labels

        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                dfsRecursive(neighbor, adj, visited, result);
            }
        }
    }

    vector<int> bruteForceDFS(int n, vector<vector<int>>& edges) {
        vector<vector<int>> adj(n);

        // Build adjacency list
        for (auto& edge : edges) {
            int u = edge[0], v = edge[1];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        vector<bool> visited(n, false);
        vector<int> result;

        // DFS starting from node 1 (index 0)
        dfsRecursive(0, adj, visited, result);

        return result;
    }

    // -------- Optimal: Iterative DFS using stack ------------
    vector<int> optimalDFS(int n, vector<vector<int>>& edges) {
        vector<vector<int>> adj(n);

        // Build adjacency list
        for (auto& edge : edges) {
            int u = edge[0], v = edge[1];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        vector<bool> visited(n, false);
        vector<int> result;
        stack<int> st;

        // Start DFS from node 1 (index 0)
        st.push(0);

        while (!st.empty()) {
            int node = st.top();
            st.pop();

            if (!visited[node]) {
                visited[node] = true;
                result.push_back(node + 1); // +1 for 1-based labels

                // Push neighbors in reverse order for correct DFS order
                for (auto it = adj[node].rbegin(); it != adj[node].rend(); ++it) {
                    if (!visited[*it]) {
                        st.push(*it);
                    }
                }
            }
        }

        return result;
    }
};

int main() {
    Solution sol;

    int n = 5;
    // Graph from the image (converted to 0-based indexing)
    vector<vector<int>> edges = {
        {0,1}, {0,2}, {1,3}, {1,4}, {3,4}
    };

    vector<int> dfsRec = sol.bruteForceDFS(n, edges);
    cout << "DFS Recursive: ";
    for (int node : dfsRec) cout << node << " ";
    cout << "\n";

    vector<int> dfsIter = sol.optimalDFS(n, edges);
    cout << "DFS Iterative: ";
    for (int node : dfsIter) cout << node << " ";
    cout << "\n";

    return 0;
}

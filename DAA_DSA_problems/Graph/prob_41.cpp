#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int timer = 1;

// DFS to find bridges
void dfs(int node, int parent, vector<int> &vis,
         vector<int> adj[], int tin[], int low[], vector<vector<int>> &bridges) {
    vis[node] = 1;
    tin[node] = low[node] = timer++;
    
    for (auto it : adj[node]) {
        if (it == parent) continue;

        if (!vis[it]) {
            dfs(it, node, vis, adj, tin, low, bridges);
            low[node] = min(low[node], low[it]);

            // Condition for bridge
            if (low[it] > tin[node]) {
                bridges.push_back({node, it});
            }
        } else {
            low[node] = min(low[node], tin[it]);
        }
    }
}

// Function to find all critical connections (bridges)
vector<vector<int>> criticalConnections(int n, vector<vector<int>> &connections) {
    vector<int> adj[n];
    for (auto &edge : connections) {
        int u = edge[0], v = edge[1];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> vis(n, 0);
    int tin[n], low[n];
    vector<vector<int>> bridges;

    // Run DFS for all components (in case graph is disconnected)
    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            dfs(i, -1, vis, adj, tin, low, bridges);
        }
    }

    return bridges;
}

int main() {
    int n = 4;
    vector<vector<int>> connections = {
        {0, 1}, {1, 2},
        {2, 0}, {1, 3}
    };

    vector<vector<int>> bridges = criticalConnections(n, connections);

    cout << "Bridges (Critical Connections): ";
    for (auto &edge : bridges) {
        cout << "[" << edge[0] << ", " << edge[1] << "] ";
    }
    cout << endl;

    return 0;
}


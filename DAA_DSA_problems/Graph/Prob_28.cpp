#include <vector>
#include <queue>
#include <climits>
#include <iostream>

using namespace std;

// Brute Force Approach: DFS to find shortest path by exploring all paths
class BruteForceSolution {
private:
    // DFS to explore all paths from current node to find minimum distance
    void dfs(int node, int src, int dist, vector<vector<int>>& adj, vector<bool>& visited, vector<int>& distances) {
        // If current distance is greater than known shortest distance, prune
        if (dist >= distances[node]) return;
        distances[node] = dist; // Update shortest distance to node
        visited[node] = true;
        // Explore all neighbors
        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                dfs(neighbor, src, dist + 1, adj, visited, distances);
            }
        }
        visited[node] = false; // Backtrack to explore other paths
    }

public:
    vector<int> shortestPath(int n, vector<vector<int>>& edges, int src) {
        // Build adjacency list
        vector<vector<int>> adj(n);
        for (const auto& edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]); // Undirected graph
        }
        // Initialize distances
        vector<int> distances(n, INT_MAX);
        vector<bool> visited(n, false);
        distances[src] = 0;
        // Run DFS from source
        dfs(src, src, 0, adj, visited, distances);
        // Set -1 for unreachable nodes
        for (int i = 0; i < n; ++i) {
            if (distances[i] == INT_MAX) {
                distances[i] = -1;
            }
        }
        return distances;
    }
};

// Optimal Approach: BFS for shortest path in unit-weight graph
class OptimalSolution {
public:
    vector<int> shortestPath(int n, vector<vector<int>>& edges, int src) {
        // Build adjacency list
        vector<vector<int>> adj(n);
        for (const auto& edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]); // Undirected graph
        }
        // Initialize distances
        vector<int> distances(n, -1);
        distances[src] = 0;
        // BFS queue
        queue<int> q;
        q.push(src);
        // BFS to find shortest paths
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            // Explore neighbors
            for (int neighbor : adj[node]) {
                if (distances[neighbor] == -1) { // Not visited
                    distances[neighbor] = distances[node] + 1;
                    q.push(neighbor);
                }
            }
        }
        return distances;
    }
};

// Main function to handle input and output
int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> edges(m, vector<int>(2));
    for (int i = 0; i < m; ++i) {
        cin >> edges[i][0] >> edges[i][1];
    }
    int src = 0; // Source is fixed at 0

    OptimalSolution opt;
    vector<int> distances = opt.shortestPath(n, edges, src);
    // Output distances
    for (int dist : distances) {
        cout << dist << " ";
    }
    cout << endl;
    return 0;
}
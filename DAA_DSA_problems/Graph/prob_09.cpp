#include <iostream>     
#include <vector>       
#include <queue>        
#include <climits>      

using namespace std;

const int MOD = 1e9 + 7;

class Solution {
public:
    // ---------- Brute Force DFS -----------
    // Explore all paths using DFS while tracking time
    // Prune paths longer than current minimum time
    void dfs(int node, int time, vector<pair<int, int>> adj[], vector<bool>& visited,
             int& minTime, int& count, int dest) {
        if (time > minTime) return;  // prune longer paths
        
        if (node == dest) {
            // Found a path to destination
            if (time < minTime) {
                // Found a new shorter path
                minTime = time;
                count = 1;  // reset count
            } else if (time == minTime) {
                // Found another shortest path with same min time
                count++;
            }
            return;
        }

        visited[node] = true;  // mark current node as visited
        for (auto& edge : adj[node]) {
            int nei = edge.first;  // neighbor node
            int wt = edge.second;  // edge weight (time)
            if (!visited[nei]) {
                dfs(nei, time + wt, adj, visited, minTime, count, dest);
            }
        }
        visited[node] = false;  // backtrack: unmark node
    }

    // Wrapper function for brute force approach
    int countPathsBruteForce(int n, vector<vector<int>>& roads) {
        // Build adjacency list: node -> {(neighbor, travel_time), ...}
        vector<pair<int, int>> adj[n];
        for (auto& r : roads) {
            adj[r[0]].emplace_back(r[1], r[2]);
            adj[r[1]].emplace_back(r[0], r[2]);
        }

        int minTime = INT_MAX;
        int count = 0;
        vector<bool> visited(n, false);
        dfs(0, 0, adj, visited, minTime, count, n - 1);  // start DFS from node 0
        return count % MOD;
    }

    // -------- Optimal (Dijkstra + Path Counting) --------
    int countPathsOptimal(int n, vector<vector<int>>& roads) {
        // Build adjacency list
        vector<pair<int, int>> adj[n];
        for (auto& r : roads) {
            adj[r[0]].emplace_back(r[1], r[2]);
            adj[r[1]].emplace_back(r[0], r[2]);
        }

        vector<long long> dist(n, LLONG_MAX);  // shortest distances to each node
        vector<int> ways(n, 0);                 // number of shortest paths to each node
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

        dist[0] = 0;
        ways[0] = 1;
        pq.emplace(0, 0);  // {distance/time, node}

        while (!pq.empty()) {
            auto [currTime, u] = pq.top(); pq.pop();

            if (currTime > dist[u]) continue;  // skip outdated entry

            // Explore neighbors of u
            for (auto& edge : adj[u]) {
                int v = edge.first;
                int wt = edge.second;
                long long newTime = currTime + wt;

                if (newTime < dist[v]) {
                    // Found shorter path to v
                    dist[v] = newTime;
                    ways[v] = ways[u];  // ways to v = ways to u
                    pq.emplace(newTime, v);
                } else if (newTime == dist[v]) {
                    // Found another shortest path to v
                    ways[v] = (ways[v] + ways[u]) % MOD;
                }
            }
        }
        return ways[n - 1];  // number of shortest paths to destination
    }
};


int main() {
    Solution sol;

    vector<vector<int>> roads = {
        {0, 6, 7},
        {0, 1, 2},
        {1, 2, 3},
        {1, 3, 3},
        {6, 3, 3},
        {3, 5, 1},
        {6, 5, 1},
        {2, 5, 1},
        {0, 4, 5},
        {4, 6, 2}
    };
    int n = 7;

    cout << "BruteForce: " << sol.countPathsBruteForce(n, roads) << endl;
    cout << "Optimal: " << sol.countPathsOptimal(n, roads) << endl;

    return 0;
}

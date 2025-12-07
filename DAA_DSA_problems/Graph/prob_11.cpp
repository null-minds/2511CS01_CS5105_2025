//P011
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

// Disjoint Set Union (Union-Find) data structure implementation
class DSU {
    vector<int> parent, size;
public:
    DSU(int n) {
        parent.resize(n);
        size.resize(n, 1);
        for (int i = 0; i < n; i++) parent[i] = i;  // Initially each node is its own parent
    }

    // Find with path compression
    int find(int a) {
        if (parent[a] == a) return a;
        return parent[a] = find(parent[a]);
    }

    // Union by size, returns true if union happened, false if already in same set
    bool unionSet(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        return true;
    }
};

class Solution {
public:
    // -------- Brute Force (Graph + DFS) --------
    int removeStonesBruteForce(vector<vector<int>>& stones) {
        int n = stones.size();
        vector<vector<int>> adj(n);

        // Build graph by connecting stones sharing the same row or column
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (stones[i][0] == stones[j][0] || stones[i][1] == stones[j][1]) {
                    adj[i].push_back(j);
                    adj[j].push_back(i);
                }
            }
        }

        vector<bool> visited(n, false);
        int connectedComponents = 0;

        // Count connected components using DFS
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                connectedComponents++;
                dfs(i, adj, visited);
            }
        }

        // Maximum stones that can be removed = total stones - number of connected components
        return n - connectedComponents;
    }

    // DFS helper function
    void dfs(int node, vector<vector<int>>& adj, vector<bool>& visited) {
        visited[node] = true;
        for (int nei : adj[node]) {
            if (!visited[nei]) dfs(nei, adj, visited);
        }
    }

    // -------- Optimal (DSU) --------
    int removeStonesOptimal(vector<vector<int>>& stones) {
        int n = stones.size();
        DSU dsu(20000); // Large enough size to accommodate offset nodes

        // Use offset to differentiate between row and column indices
        // Union stones by their rows and columns
        for (auto& stone : stones) {
            int row = stone[0];
            int col = stone[1] + 10000; // offset columns to avoid collision with rows
            dsu.unionSet(row, col);
        }

        unordered_set<int> uniqueParents;
        // Find unique connected components by checking distinct parents of rows
        for (auto& stone : stones) {
            uniqueParents.insert(dsu.find(stone[0]));
        }

        // Maximum stones removed = total stones - number of connected components
        return n - (int)uniqueParents.size();
    }
};

int main() {
    Solution sol;

    vector<vector<int>> stones = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 2},
        {2, 1},
        {2, 2}
    };

    cout << "Brute Force result: " << sol.removeStonesBruteForce(stones) << endl;
    cout << "Optimal DSU result: " << sol.removeStonesOptimal(stones) << endl;

    return 0;
}

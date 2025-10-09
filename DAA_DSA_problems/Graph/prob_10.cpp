//P010
#include <iostream>
#include <vector>

using namespace std;

class DSU {
    vector<int> parent, size;
public:
    DSU(int n) {
        parent.resize(n);
        size.resize(n, 1);
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int a) {
        if (parent[a] == a) return a;
        return parent[a] = find(parent[a]);  // path compression
    }

    bool unionSet(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;  // already connected
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        return true;
    }
};

class Solution {
public:
    // Brute Force (basic DSU)
    int makeConnectedBruteForce(int n, vector<vector<int>>& edges) {
        DSU dsu(n);
        int extraEdges = 0;
        for (auto& e : edges) {
            if (!dsu.unionSet(e[0], e[1])) {
                // edge connecting same component is extra
                extraEdges++;
            }
        }

        // Count components
        int components = 0;
        for (int i = 0; i < n; i++) {
            if (dsu.find(i) == i) components++;
        }

        int needed = components - 1;
        if (extraEdges >= needed) return needed;
        return -1;
    }

    // Optimal (same approach but cleaner)
    int makeConnectedOptimal(int n, vector<vector<int>>& edges) {
        DSU dsu(n);
        int extraEdges = 0;
        for (auto& e : edges) {
            if (!dsu.unionSet(e[0], e[1])) extraEdges++;
        }

        int components = 0;
        for (int i = 0; i < n; i++) {
            if (dsu.find(i) == i) components++;
        }

        int needed = components - 1;
        return (extraEdges >= needed) ? needed : -1;
    }
};


int main() {
    Solution sol;

    // vector<vector<int>> edges1 = { {0,1}, {0,2}, {1,2} };
    // int n1 = 4;
    // cout << "BruteForce Result 1: " << sol.makeConnectedBruteForce(n1, edges1) << endl;
    // cout << "Optimal Result 1: " << sol.makeConnectedOptimal(n1, edges1) << endl;

    vector<vector<int>> edges2 = {
        {0,1}, {0,2}, {0,3}, {1,2}, {2,3}, {4,5}, {5,6}, {7,8}
    };
    int n2 = 9;
    cout << "BruteForce Result 2: " << sol.makeConnectedBruteForce(n2, edges2) << endl;
    cout << "Optimal Result 2: " << sol.makeConnectedOptimal(n2, edges2) << endl;

    return 0;
}

#include <bits/stdc++.h>
using namespace std;

// 🔹 Disjoint Set (Union-Find) for Optimal Kruskal
class DisjointSet {
    vector<int> parent, rank;
public:
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    int findParent(int u) {
        if (u == parent[u]) return u;
        return parent[u] = findParent(parent[u]); // Path compression
    }

    void unionByRank(int u, int v) {
        int pu = findParent(u);
        int pv = findParent(v);
        if (pu == pv) return;

        if (rank[pu] < rank[pv]) parent[pu] = pv;
        else if (rank[pv] < rank[pu]) parent[pv] = pu;
        else {
            parent[pv] = pu;
            rank[pu]++;
        }
    }
};

// 🔹 Brute Force Kruskal (using STL sort and cycle check)
int kruskalBruteForce(int V, vector<vector<int>>& edges) {
    // Sort edges by weight
    sort(edges.begin(), edges.end(), [](auto &a, auto &b) {
        return a[2] < b[2];
    });

    vector<int> parent(V);
    for (int i = 0; i < V; i++) parent[i] = i;

    // Simple find parent function
    function<int(int)> findParent = [&](int u) {
        if (parent[u] == u) return u;
        return parent[u] = findParent(parent[u]);
    };

    int mstWeight = 0;

    for (auto &edge : edges) {
        int u = edge[0], v = edge[1], w = edge[2];
        int pu = findParent(u);
        int pv = findParent(v);

        if (pu != pv) {
            mstWeight += w;
            parent[pu] = pv; // union
        }
    }

    return mstWeight;
}

// 🔹 Optimal Kruskal using Disjoint Set
int kruskalOptimal(int V, vector<vector<int>>& edges) {
    // Sort edges by weight
    sort(edges.begin(), edges.end(), [](auto &a, auto &b) {
        return a[2] < b[2];
    });

    DisjointSet ds(V);
    int mstWeight = 0;

    for (auto &edge : edges) {
        int u = edge[0], v = edge[1], w = edge[2];
        if (ds.findParent(u) != ds.findParent(v)) {
            mstWeight += w;
            ds.unionByRank(u, v);
        }
    }

    return mstWeight;
}

int main() {
    int V = 5;
    vector<vector<int>> edges = {
        {0, 1, 2}, {0, 3, 6}, {1, 2, 3}, {1, 3, 8}, {1, 4, 5}, {4, 2, 7}
    };

    cout << "Brute Force MST Sum: " << kruskalBruteForce(V, edges) << endl;
    cout << "Optimal MST Sum: " << kruskalOptimal(V, edges) << endl;

    return 0;
}

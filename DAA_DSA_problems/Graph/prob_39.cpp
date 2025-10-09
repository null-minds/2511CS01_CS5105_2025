#include <bits/stdc++.h>
using namespace std;

// 🔹 Disjoint Set for Optimal Solution
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
        if (parent[u] == u) return u;
        return parent[u] = findParent(parent[u]); // path compression
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

// 🔹 Brute Force Approach (DFS)
int dfs(vector<vector<int>>& grid, int i, int j, vector<vector<bool>>& visited) {
    int n = grid.size(), m = grid[0].size();
    if (i < 0 || i >= n || j < 0 || j >= m || visited[i][j] || grid[i][j] == 0) return 0;

    visited[i][j] = true;

    dfs(grid, i + 1, j, visited);
    dfs(grid, i - 1, j, visited);
    dfs(grid, i, j + 1, visited);
    dfs(grid, i, j - 1, visited);

    return 1;
}

vector<int> numIslandsBruteForce(int n, int m, vector<pair<int,int>>& positions) {
    vector<int> res;
    vector<vector<int>> grid(n, vector<int>(m, 0));

    for (auto &p : positions) {
        grid[p.first][p.second] = 1;
        int count = 0;
        vector<vector<bool>> visited(n, vector<bool>(m, false));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j] && !visited[i][j])
                    count += dfs(grid, i, j, visited);
            }
        }
        res.push_back(count);
    }

    return res;
}

// 🔹 Optimal Approach using Union-Find
vector<int> numIslandsOptimal(int n, int m, vector<pair<int,int>>& positions) {
    vector<int> res;
    DisjointSet ds(n * m);
    vector<vector<int>> grid(n, vector<int>(m, 0));
    int islands = 0;
    vector<int> dx = {-1, 0, 1, 0};
    vector<int> dy = {0, 1, 0, -1};

    for (auto &p : positions) {
        int r = p.first, c = p.second;
        if (grid[r][c] == 1) {
            res.push_back(islands);
            continue; // skip duplicate operation
        }

        grid[r][c] = 1;
        islands++;
        int id1 = r * m + c;

        for (int k = 0; k < 4; k++) {
            int nr = r + dx[k], nc = c + dy[k];
            if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] == 1) {
                int id2 = nr * m + nc;
                if (ds.findParent(id1) != ds.findParent(id2)) {
                    ds.unionByRank(id1, id2);
                    islands--;
                }
            }
        }

        res.push_back(islands);
    }

    return res;
}

int main() {
    int n = 4, m = 5;
    vector<pair<int,int>> positions = {{1,1},{0,1},{3,3},{3,4}};

    cout << "Brute Force Output: ";
    auto brute = numIslandsBruteForce(n, m, positions);
    for (int x : brute) cout << x << " ";
    cout << "\n";

    cout << "Optimal Output: ";
    auto optimal = numIslandsOptimal(n, m, positions);
    for (int x : optimal) cout << x << " ";
    cout << "\n";

    return 0;
}

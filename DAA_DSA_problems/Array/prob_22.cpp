#include <bits/stdc++.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;
public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    // ---------------- Brute Force: DFS ----------------
    void dfs(int node, vector<bool> &visited, stack<int> &st) {
        visited[node] = true;
        for (int nbr : adj[node]) {
            if (!visited[nbr]) {
                dfs(nbr, visited, st);
            }
        }
        st.push(node);
    }

    vector<int> topoSortDFS() {
        vector<bool> visited(V, false);
        stack<int> st;
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                dfs(i, visited, st);
            }
        }
        vector<int> result;
        while (!st.empty()) {
            result.push_back(st.top());
            st.pop();
        }
        return result;
    }

    // ---------------- Optimal: Kahn’s Algorithm ----------------
    vector<int> topoSortKahn() {
        vector<int> inDegree(V, 0);
        for (int i = 0; i < V; i++) {
            for (int nbr : adj[i]) {
                inDegree[nbr]++;
            }
        }

        queue<int> q;
        for (int i = 0; i < V; i++) {
            if (inDegree[i] == 0) q.push(i);
        }

        vector<int> result;
        while (!q.empty()) {
            int node = q.front(); q.pop();
            result.push_back(node);

            for (int nbr : adj[node]) {
                inDegree[nbr]--;
                if (inDegree[nbr] == 0) q.push(nbr);
            }
        }
        return result;
    }
};

int main() {
    int V = 6, E = 6;
    Graph g(V);

    // Input edges
    g.addEdge(5, 0);
    g.addEdge(5, 2);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(2, 3);
    g.addEdge(3, 1);

    // Brute Force
    vector<int> res1 = g.topoSortDFS();
    cout << "DFS Topological Sort: ";
    for (int x : res1) cout << x << " ";
    cout << endl;

    // Optimal
    vector<int> res2 = g.topoSortKahn();
    cout << "Kahn's Topological Sort: ";
    for (int x : res2) cout << x << " ";
    cout << endl;

    return 0;
}

#include <iostream>
#include <vector>
#include <stack>
using namespace std;

// Step 1: DFS to fill stack according to finishing time
void dfs(int node, vector<int> &vis, vector<vector<int>> &adj, stack<int> &st) {
    vis[node] = 1;
    for (auto it : adj[node]) {
        if (!vis[it]) {
            dfs(it, vis, adj, st);
        }
    }
    st.push(node);
}

// Step 2: DFS on transposed graph
void dfsTranspose(int node, vector<int> &vis, vector<vector<int>> &adjT) {
    vis[node] = 1;
    for (auto it : adjT[node]) {
        if (!vis[it]) {
            dfsTranspose(it, vis, adjT);
        }
    }
}

// Function to find number of strongly connected components
int kosaraju(int V, vector<vector<int>> &adj) {
    vector<int> vis(V, 0);
    stack<int> st;

    // Step 1: Fill nodes in stack according to finishing times
    for (int i = 0; i < V; i++) {
        if (!vis[i]) {
            dfs(i, vis, adj, st);
        }
    }

    // Step 2: Transpose graph
    vector<vector<int>> adjT(V);
    for (int i = 0; i < V; i++) {
        for (auto it : adj[i]) {
            adjT[it].push_back(i); // reverse edge
        }
    }

    // Step 3: Process nodes in order of stack in transpose graph
    fill(vis.begin(), vis.end(), 0);
    int scc = 0;
    while (!st.empty()) {
        int node = st.top();
        st.pop();
        if (!vis[node]) {
            scc++;
            dfsTranspose(node, vis, adjT);
        }
    }

    return scc;
}

int main() {
    int V = 5;
    vector<vector<int>> adj(V);

    // Example edges
    vector<pair<int, int>> edges = {
        {1, 0}, {0, 2}, {2, 1}, {0, 3}, {3, 4}
    };

    // Build adjacency list
    for (auto &e : edges) {
        adj[e.first].push_back(e.second);
    }

    int ans = kosaraju(V, adj);
    cout << "The number of strongly connected components is: " << ans << endl;

    return 0;
}


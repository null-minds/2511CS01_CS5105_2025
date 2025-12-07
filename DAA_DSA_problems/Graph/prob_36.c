#include <bits/stdc++.h>
using namespace std;

int primMST(int V, vector<vector<pair<int, int>>>& adj) {
    // Min-heap {weight, node}
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    vector<bool> visited(V, false);

    int mstWeight = 0;
    pq.push({0, 0}); // start from node 0 with weight 0

    while (!pq.empty()) {
        auto [wt, u] = pq.top();
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;
        mstWeight += wt;

        for (auto [v, weight] : adj[u]) {
            if (!visited[v]) {
                pq.push({weight, v});
            }
        }
    }

    return mstWeight;
}

int main() {
    int V, E;
    cout << "Enter number of vertices and edges: ";
    cin >> V >> E;

    vector<vector<pair<int, int>>> adj(V);

    cout << "Enter edges in format (u v w):\n";
    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // undirected graph
    }

    int result = primMST(V, adj);
    cout << "MST total weight (using Prim's Algorithm) = " << result << endl;

    return 0;
}

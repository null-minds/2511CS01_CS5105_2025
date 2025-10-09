#include <bits/stdc++.h>
using namespace std;

// Optimal Bellman-Ford with early stopping
vector<int> bellmanFord(int V, int E, int src, vector<tuple<int,int,int>> &edges) {
    vector<int> dist(V, 1e9);
    dist[src] = 0;

    // Relax edges with early termination
    for (int i = 0; i < V - 1; i++) {
        bool changed = false;
        for (int j = 0; j < E; j++) {
            int u = get<0>(edges[j]);
            int v = get<1>(edges[j]);
            int w = get<2>(edges[j]);
            if (dist[u] != 1e9 && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                changed = true;
            }
        }
        if (!changed) break; 
    }

    // Check for negative cycle
    for (int j = 0; j < E; j++) {
        int u = get<0>(edges[j]);
        int v = get<1>(edges[j]);
        int w = get<2>(edges[j]);
        if (dist[u] != 1e9 && dist[u] + w < dist[v]) {
            return {-1}; 
        }
    }
    return dist;
}

int main() {
    int V, E, src;
    cin >> V >> E >> src;

    vector<tuple<int,int,int>> edges;
    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }

    vector<int> result = bellmanFord(V, E, src, edges);

    if (result.size() == 1 && result[0] == -1) {
        cout << "Graph contains a negative cycle" << endl;
    } else {
        cout << "Shortest distances from source " << src << ": ";
        for (int d : result) cout << d << " ";
        cout << endl;
    }

    return 0;
}

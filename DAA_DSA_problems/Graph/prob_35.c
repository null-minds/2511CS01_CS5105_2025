#include <bits/stdc++.h>
using namespace std;

// Approach using Floyd-Warshall
int findTheCity(int n, vector<vector<int>>& edges, int distanceThreshold) {
    vector<vector<int>> dist(n, vector<int>(n, 1e9));

    // Distance from a city to itself = 0
    for(int i = 0; i < n; i++) dist[i][i] = 0;

    // Fill given edges
    for(auto &e : edges) {
        int u = e[0], v = e[1], w = e[2];
        dist[u][v] = w;
        dist[v][u] = w;
    }

    // Floyd-Warshall
    for(int k = 0; k < n; k++) {
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                if(dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    int city = -1, minReach = INT_MAX;

    // Count reachable cities for each node
    for(int i = 0; i < n; i++) {
        int count = 0;
        for(int j = 0; j < n; j++) {
            if(i != j && dist[i][j] <= distanceThreshold) {
                count++;
            }
        }
        if(count <= minReach) {
            minReach = count;
            city = i; // take greatest index in case of tie
        }
    }

    return city;
}

int main() {
    int n, m, distanceThreshold;
    cin >> n >> m >> distanceThreshold;

    vector<vector<int>> edges(m, vector<int>(3));
    for(int i = 0; i < m; i++) {
        cin >> edges[i][0] >> edges[i][1] >> edges[i][2];
    }

    cout << findTheCity(n, edges, distanceThreshold) << endl;

    return 0;
}

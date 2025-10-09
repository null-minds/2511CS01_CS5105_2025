#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

// Brute-force approach for Dijkstra's Algorithm
// This method finds the minimum distance vertex by scanning the entire
// distances array in each iteration.
// Time Complexity: O(V^2)
// Space Complexity: O(V)
vector<int> dijkstraBruteForce(int V, const vector<vector<pair<int, int>>>& adj, int S) {
    vector<int> distances(V, numeric_limits<int>::max());
    vector<bool> visited(V, false);
    distances[S] = 0;

    for (int i = 0; i < V; ++i) {
        int minDistance = numeric_limits<int>::max();
        int minVertex = -1;

        // Find the unvisited vertex with the minimum distance
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && distances[v] < minDistance) {
                minDistance = distances[v];
                minVertex = v;
            }
        }

        // If no reachable vertex is left, break
        if (minVertex == -1) {
            break;
        }

        visited[minVertex] = true;

        // Relax neighbors
        for (const auto& edge : adj[minVertex]) {
            int neighbor = edge.first;
            int weight = edge.second;
            if (distances[minVertex] != numeric_limits<int>::max() && distances[minVertex] + weight < distances[neighbor]) {
                distances[neighbor] = distances[minVertex] + weight;
            }
        }
    }
    return distances;
}

// Optimal approach for Dijkstra's Algorithm using a min-priority queue
// This method efficiently finds the vertex with the minimum distance,
// making it suitable for sparse graphs.
// Time Complexity: O(E log V)
// Space Complexity: O(V + E)
vector<int> dijkstraOptimal(int V, const vector<vector<pair<int, int>>>& adj, int S) {
    vector<int> distances(V, numeric_limits<int>::max());
    
    // The priority queue stores pairs of {distance, vertex}
    // We use greater<pair<int, int>> to make it a min-priority queue
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    distances[S] = 0;
    pq.push({0, S});

    while (!pq.empty()) {
        int currentDist = pq.top().first;
        int currentVertex = pq.top().second;
        pq.pop();

        if (currentDist > distances[currentVertex]) {
            continue;
        }

        for (const auto& edge : adj[currentVertex]) {
            int neighbor = edge.first;
            int weight = edge.second;

            if (distances[currentVertex] + weight < distances[neighbor]) {
                distances[neighbor] = distances[currentVertex] + weight;
                pq.push({distances[neighbor], neighbor});
            }
        }
    }
    return distances;
}

int main() {
    // Example 1: Bipartite Graph
    int V1 = 2;
    vector<vector<pair<int, int>>> adj1(V1);
    adj1[0].push_back({1, 9});
    adj1[1].push_back({0, 9});
    int S1 = 0;

    cout << "Example 1:" << endl;
    cout << "Brute-Force Result: ";
    vector<int> distBruteForce1 = dijkstraBruteForce(V1, adj1, S1);
    for (int dist : distBruteForce1) {
        cout << dist << " ";
    }
    cout << endl;

    cout << "Optimal Result: ";
    vector<int> distOptimal1 = dijkstraOptimal(V1, adj1, S1);
    for (int dist : distOptimal1) {
        cout << dist << " ";
    }
    cout << endl;

    // Example 2: More complex graph
    int V2 = 6;
    vector<vector<pair<int, int>>> adj2(V2);
    adj2[0].push_back({1, 4});
    adj2[0].push_back({2, 1});
    adj2[1].push_back({0, 4});
    adj2[1].push_back({2, 2});
    adj2[1].push_back({3, 5});
    adj2[2].push_back({0, 1});
    adj2[2].push_back({1, 2});
    adj2[2].push_back({3, 8});
    adj2[2].push_back({4, 10});
    adj2[3].push_back({1, 5});
    adj2[3].push_back({2, 8});
    adj2[3].push_back({4, 2});
    adj2[3].push_back({5, 6});
    adj2[4].push_back({2, 10});
    adj2[4].push_back({3, 2});
    adj2[4].push_back({5, 3});
    adj2[5].push_back({3, 6});
    adj2[5].push_back({4, 3});
    int S2 = 0;

    cout << "\nExample 2:" << endl;
    cout << "Brute-Force Result: ";
    vector<int> distBruteForce2 = dijkstraBruteForce(V2, adj2, S2);
    for (int dist : distBruteForce2) {
        cout << dist << " ";
    }
    cout << endl;
    
    cout << "Optimal Result: ";
    vector<int> distOptimal2 = dijkstraOptimal(V2, adj2, S2);
    for (int dist : distOptimal2) {
        cout << dist << " ";
    }
    cout << endl;

    return 0;
}

#include <bits/stdc++.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;
public:
    Graph(int V) {
        this->V = V;
        adj.resize(V+1); // 1-based indexing
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    // ---------------- Brute Force ----------------
    bool dfsBrute(int node, vector<bool> &visited, vector<bool> &path) {
        visited[node] = true;
        path[node] = true;

        for (int nbr : adj[node]) {
            if (!visited[nbr]) {
                if (dfsBrute(nbr, visited, path)) return true;
            } else if (path[nbr]) {
                return true;
            }
        }

        path[node] = false;
        return false;
    }

    bool detectCycleBrute() {
        for (int i = 1; i <= V; i++) {
            vector<bool> visited(V+1, false), path(V+1, false);
            if (dfsBrute(i, visited, path)) return true;
        }
        return false;
    }

    // ---------------- Optimal ----------------
    bool dfsOptimal(int node, vector<bool> &visited, vector<bool> &recStack) {
        visited[node] = true;
        recStack[node] = true;

        for (int nbr : adj[node]) {
            if (!visited[nbr]) {
                if (dfsOptimal(nbr, visited, recStack)) return true;
            } else if (recStack[nbr]) {
                return true;
            }
        }

        recStack[node] = false;
        return false;
    }

    bool detectCycleOptimal() {
        vector<bool> visited(V+1, false), recStack(V+1, false);
        for (int i = 1; i <= V; i++) {
            if (!visited[i]) {
                if (dfsOptimal(i, visited, recStack)) return true;
            }
        }
        return false;
    }
};

int main() {
    int N = 10, E = 11;
    Graph g(N);

    // Input edges as per example
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(3, 7);
    g.addEdge(4, 5);
    g.addEdge(5, 6);
    g.addEdge(7, 5);
    g.addEdge(2, 8);
    g.addEdge(8, 9);
    g.addEdge(9, 10);
    g.addEdge(10, 8);

    cout << "Brute Force: " << (g.detectCycleBrute() ? "true" : "false") << endl;
    cout << "Optimal: " << (g.detectCycleOptimal() ? "true" : "false") << endl;

    return 0;
}

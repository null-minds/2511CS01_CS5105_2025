#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Brute-force approach: conceptualized as a traversal on an adjacency matrix.
// This is less efficient because each node's neighbors are found by iterating
// through all 'V' possible vertices in an inner loop.
// Time Complexity: O(V^2) for a dense graph using an adjacency matrix.
// Space Complexity: O(V) for the visited/color array.
bool isBipartiteBruteForce(int V, const vector<vector<int>>& adjMatrix) {
    vector<int> color(V, -1);

    function<bool(int, int, int)> dfsTraversal = 
        [&](int node, int parent, int currentColor) {
        color[node] = currentColor;
        
        for (int neighbor = 0; neighbor < V; ++neighbor) {
            if (adjMatrix[node][neighbor] == 1) { // Check for an edge
                if (color[neighbor] == -1) {
                    if (!dfsTraversal(neighbor, node, 1 - currentColor)) {
                        return false;
                    }
                } else if (neighbor != parent) {
                    if (color[neighbor] == currentColor) {
                        return false;
                    }
                }
            }
        }
        return true;
    };

    for (int i = 0; i < V; ++i) {
        if (color[i] == -1) {
            if (!dfsTraversal(i, -1, 0)) {
                return false;
            }
        }
    }
    return true;
}

// Optimal approach (DFS): Uses a single traversal to color and check the graph.
// It is efficient for both sparse and dense graphs when using an adjacency list.
// Time Complexity: O(V + E), where V is vertices and E is edges.
// Space Complexity: O(V) for the color array and recursion stack.
bool isBipartiteOptimal(int V, const vector<vector<int>>& adjList) {
    vector<int> color(V, -1);

    function<bool(int, int, int)> dfsColor = 
        [&](int node, int parent, int currentColor) {
        color[node] = currentColor;

        for (int neighbor : adjList[node]) {
            if (color[neighbor] == -1) {
                if (!dfsColor(neighbor, node, 1 - currentColor)) {
                    return false;
                }
            } else if (color[neighbor] == currentColor) {
                return false;
            }
        }
        return true;
    };

    for (int i = 0; i < V; ++i) {
        if (color[i] == -1) {
            if (!dfsColor(i, -1, 0)) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    // Example 1: Bipartite Graph (a tree structure)
    int V1 = 5;
    vector<vector<int>> adjList1(V1);
    adjList1[0].push_back(1);
    adjList1[1].push_back(0);
    adjList1[1].push_back(2);
    adjList1[2].push_back(1);
    adjList1[2].push_back(3);
    adjList1[3].push_back(2);
    adjList1[3].push_back(4);
    adjList1[4].push_back(3);

    cout << "Example 1: Bipartite Graph (Tree)" << endl;
    cout << "Optimal (DFS) result: " << (isBipartiteOptimal(V1, adjList1) ? "1" : "0") << endl;
    
    // Example 2: Non-Bipartite Graph (a triangle)
    int V2 = 3;
    vector<vector<int>> adjList2(V2);
    adjList2[0].push_back(1);
    adjList2[1].push_back(0);
    adjList2[1].push_back(2);
    adjList2[2].push_back(1);
    adjList2[2].push_back(0);
    adjList2[0].push_back(2);

    cout << "\nExample 2: Non-Bipartite Graph (Triangle)" << endl;
    cout << "Optimal (DFS) result: " << (isBipartiteOptimal(V2, adjList2) ? "1" : "0") << endl;

    return 0;
}

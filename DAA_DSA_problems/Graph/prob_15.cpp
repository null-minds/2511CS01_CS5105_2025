#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Brute-force approach for cycle detection using a DFS-like traversal.
// It iterates through all vertices and their neighbors, but without
// optimizing for graph representation. This is functionally equivalent
// to the optimal DFS but is labeled brute-force for conceptual clarity.
// Time Complexity: O(V^2) for adjacency matrix representation
// Space Complexity: O(V)
bool hasCycleBruteForce(int start, int parent, const vector<vector<int>>& adjMatrix, vector<bool>& visited) {
    visited[start] = true;
    int V = adjMatrix.size();

    for (int neighbor = 0; neighbor < V; ++neighbor) {
        if (adjMatrix[start][neighbor] == 1) { // Check for a direct edge
            if (!visited[neighbor]) {
                if (hasCycleBruteForce(neighbor, start, adjMatrix, visited)) {
                    return true;
                }
            } else if (neighbor != parent) {
                return true;
            }
        }
    }
    return false;
}

// Optimal approach for cycle detection using DFS.
// It recursively explores the graph as deep as possible before backtracking.
// Time Complexity: O(V + E) for adjacency list
// Space Complexity: O(V) for the recursion stack
bool hasCycleDFS(int node, int parent, const vector<vector<int>>& adjList, vector<bool>& visited) {
    visited[node] = true;

    for (int neighbor : adjList[node]) {
        if (!visited[neighbor]) {
            if (hasCycleDFS(neighbor, node, adjList, visited)) {
                return true;
            }
        } else if (neighbor != parent) {
            return true;
        }
    }
    return false;
}

int main() {
    // --- Example 1: Graph with a cycle (0-1-2-0)
    int numVertices1 = 3;
    vector<vector<int>> adjList1 = {{1, 2}, {0}, {0}};
    vector<vector<int>> adjMatrix1 = {{0, 1, 1}, {1, 0, 0}, {1, 0, 0}};

    cout << "Example 1 (3 vertices, 2 edges):" << endl;
    vector<bool> visited1(numVertices1, false);
    cout << "  Brute-Force (DFS on Matrix): " << (hasCycleBruteForce(0, -1, adjMatrix1, visited1) ? "Cycle detected." : "No cycle detected.") << endl;
    
    vector<bool> visited2(numVertices1, false);
    cout << "  Optimal (DFS on List): " << (hasCycleDFS(0, -1, adjList1, visited2) ? "Cycle detected." : "No cycle detected.") << endl;
    
    // --- Example 2: Graph without a cycle (a tree structure)
    int numVertices2 = 4;
    vector<vector<int>> adjList2 = {{1}, {0, 2}, {1, 3}, {2}};
    vector<vector<int>> adjMatrix2 = {{0, 1, 0, 0}, {1, 0, 1, 0}, {0, 1, 0, 1}, {0, 0, 1, 0}};

    cout << "\nExample 2 (4 vertices, 3 edges):" << endl;
    vector<bool> visited4(numVertices2, false);
    cout << "  Brute-Force (DFS on Matrix): " << (hasCycleBruteForce(0, -1, adjMatrix2, visited4) ? "Cycle detected." : "No cycle detected.") << endl;
    
    vector<bool> visited5(numVertices2, false);
    cout << "  Optimal (DFS on List): " << (hasCycleDFS(0, -1, adjList2, visited5) ? "Cycle detected." : "No cycle detected.") << endl;
    
    return 0;
}

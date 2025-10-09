#include <iostream>
#include <vector>
#include <queue>
#include <set>

using namespace std;

vector<int> bfsAdjacencyMatrix(const vector<vector<int>>& adjMatrix, int startNode) {
    int nodeCount = adjMatrix.size();
    vector<int> traversalOrder;
    if (nodeCount == 0) return traversalOrder;

    queue<int> nodeQueue;
    set<int> visitedNodes;

    nodeQueue.push(startNode);
    visitedNodes.insert(startNode);

    while (!nodeQueue.empty()) {
        int currentNode = nodeQueue.front();
        nodeQueue.pop();
        traversalOrder.push_back(currentNode);

        // Iterate through all nodes to find neighbors
        for (int neighbor = 0; neighbor < nodeCount; ++neighbor) {
            // Check for an edge and if the neighbor has not been visited
            if (adjMatrix[currentNode][neighbor] == 1 && visitedNodes.find(neighbor) == visitedNodes.end()) {
                visitedNodes.insert(neighbor);
                nodeQueue.push(neighbor);
            }
        }
    }
    return traversalOrder;
}

vector<int> bfsAdjacencyList(const vector<vector<int>>& adjList, int startNode) {
    vector<int> traversalOrder;
    if (adjList.empty()) return traversalOrder;

    queue<int> nodeQueue;
    set<int> visitedNodes;

    nodeQueue.push(startNode);
    visitedNodes.insert(startNode);

    while (!nodeQueue.empty()) {
        int currentNode = nodeQueue.front();
        nodeQueue.pop();
        traversalOrder.push_back(currentNode);

        // Iterate only through the neighbors stored in the list
        for (int neighbor : adjList[currentNode]) {
            if (visitedNodes.find(neighbor) == visitedNodes.end()) {
                visitedNodes.insert(neighbor);
                nodeQueue.push(neighbor);
            }
        }
    }
    return traversalOrder;
}

int main() {
    // Example 1: Matrix and List for the same graph
    vector<vector<int>> graph1_matrix = {
        {0, 1, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0, 0},
        {0, 0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0, 0},
        {1, 0, 0, 0, 0, 0}
    };
    vector<vector<int>> graph1_list = {
        {1, 5},
        {0, 2},
        {1, 3},
        {2, 4},
        {3},
        {0}
    };
    int startNode1 = 0; // Represents Node 1

    vector<int> result1_matrix = bfsAdjacencyMatrix(graph1_matrix, startNode1);
    cout << "Example 1 (Matrix): ";
    for (int node : result1_matrix) {
        cout << node + 1 << " ";
    }
    cout << endl;
    
    vector<int> result1_list = bfsAdjacencyList(graph1_list, startNode1);
    cout << "Example 1 (List): ";
    for (int node : result1_list) {
        cout << node + 1 << " ";
    }
    cout << endl;

    // Example 2: Matrix and List for a larger graph
    vector<vector<int>> graph2_matrix = {
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0},
        {0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    vector<vector<int>> graph2_list = {
        {1, 10},
        {0, 2, 6},
        {1, 3, 7},
        {2, 4},
        {3},
        {6, 8},
        {1, 5, 7, 9},
        {2, 6, 8},
        {5, 7},
        {6},
        {0}
    };
    int startNode2 = 0; // Represents Node 1

    vector<int> result2_matrix = bfsAdjacencyMatrix(graph2_matrix, startNode2);
    cout << "Example 2 (Matrix): ";
    for (int node : result2_matrix) {
        cout << node + 1 << " ";
    }
    cout << endl;
    
    vector<int> result2_list = bfsAdjacencyList(graph2_list, startNode2);
    cout << "Example 2 (List): ";
    for (int node : result2_list) {
        cout << node + 1 << " ";
    }
    cout << endl;

    return 0;
}

#include <iostream>
#include <vector>

using namespace std;

void findConnectedComponent(int currentNode, int totalNodes, const vector<vector<int>>& adjacencyMatrix, vector<bool>& visited) {
    visited[currentNode] = true;
    
    for (int nextNode = 0; nextNode < totalNodes; ++nextNode) {
        if (adjacencyMatrix[currentNode][nextNode] == 1 && !visited[nextNode]) {
            findConnectedComponent(nextNode, totalNodes, adjacencyMatrix, visited);
        }
    }
}

int countProvincesBruteForce(const vector<vector<int>>& isConnected) {
    int totalNodes = isConnected.size();
    if (totalNodes == 0) return 0;
    
    vector<bool> visited(totalNodes, false);
    int provinceCount = 0;
    
    for (int i = 0; i < totalNodes; ++i) {
        if (!visited[i]) {
            provinceCount++;
            findConnectedComponent(i, totalNodes, isConnected, visited);
        }
    }
    return provinceCount;
}

void dfsTraversal(int currentNode, int totalNodes, const vector<vector<int>>& isConnected, vector<bool>& visited) {
    visited[currentNode] = true;
    
    for (int nextNode = 0; nextNode < totalNodes; ++nextNode) {
        if (isConnected[currentNode][nextNode] == 1 && !visited[nextNode]) {
            dfsTraversal(nextNode, totalNodes, isConnected, visited);
        }
    }
}

int countProvincesOptimal(const vector<vector<int>>& isConnected) {
    int totalNodes = isConnected.size();
    if (totalNodes == 0) return 0;
    
    vector<bool> visited(totalNodes, false);
    int provinceCount = 0;
    
    for (int i = 0; i < totalNodes; ++i) {
        if (!visited[i]) {
            provinceCount++;
            dfsTraversal(i, totalNodes, isConnected, visited);
        }
    }
    return provinceCount;
}

int main() {
    vector<vector<int>> isConnected = {
        {1, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 1, 1},
        {0, 0, 0, 1, 1}
    };
    
    cout << "Number of provinces (Brute Force): " << countProvincesBruteForce(isConnected) << endl;
    cout << "Number of provinces (Optimal): " << countProvincesOptimal(isConnected) << endl;
    
    return 0;
}

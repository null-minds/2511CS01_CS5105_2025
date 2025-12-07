#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

// Brute-force approach using a recursive DFS. This method explores
// all possible paths, leading to high time complexity.
// Time Complexity: O(2^(N*M)) in the worst case, as it explores all paths.
// Space Complexity: O(N*M) for the recursion stack and visited array.
int bruteForceDFS(const vector<vector<int>>& grid, int n, int m, int r, int c, int destR, int destC, vector<vector<bool>>& visited) {
    if (r < 0 || r >= n || c < 0 || c >= m || grid[r][c] == 0 || visited[r][c]) {
        return numeric_limits<int>::max();
    }
    
    if (r == destR && c == destC) {
        return 0;
    }
    
    visited[r][c] = true;
    
    int minDistance = numeric_limits<int>::max();
    int delRow[] = {-1, 1, 0, 0};
    int delCol[] = {0, 0, -1, 1};
    
    for (int i = 0; i < 4; ++i) {
        int nextR = r + delRow[i];
        int nextC = c + delCol[i];
        
        int dist = bruteForceDFS(grid, n, m, nextR, nextC, destR, destC, visited);
        
        if (dist != numeric_limits<int>::max()) {
            minDistance = min(minDistance, 1 + dist);
        }
    }
    
    visited[r][c] = false; // Backtrack
    return minDistance;
}

// Optimal approach using Breadth-First Search (BFS).
// This is the ideal algorithm for finding the shortest path in an unweighted grid.
// Time Complexity: O(N*M)
// Space Complexity: O(N*M)
int optimalBFS(const vector<vector<int>>& grid, int n, int m, pair<int, int> source, pair<int, int> destination) {
    if (grid[source.first][source.second] == 0 || grid[destination.first][destination.second] == 0) {
        return -1;
    }
    
    queue<tuple<int, int, int>> q; // {distance, row, col}
    q.push({0, source.first, source.second});
    
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    visited[source.first][source.second] = true;
    
    int delRow[] = {-1, 1, 0, 0};
    int delCol[] = {0, 0, -1, 1};
    
    while (!q.empty()) {
        auto [dist, r, c] = q.front();
        q.pop();
        
        if (r == destination.first && c == destination.second) {
            return dist;
        }
        
        for (int i = 0; i < 4; ++i) {
            int nextR = r + delRow[i];
            int nextC = c + delCol[i];
            
            if (nextR >= 0 && nextR < n && nextC >= 0 && nextC < m && grid[nextR][nextC] == 1 && !visited[nextR][nextC]) {
                visited[nextR][nextC] = true;
                q.push({dist + 1, nextR, nextC});
            }
        }
    }
    
    return -1;
}

int main() {
    vector<vector<int>> grid = {
        {1, 1, 1, 1},
        {1, 1, 0, 1},
        {1, 1, 1, 1},
        {1, 1, 0, 0},
        {1, 0, 0, 1}
    };
    int n = 5, m = 4;
    pair<int, int> source = {0, 1};
    pair<int, int> destination = {2, 2};

    cout << "Shortest Distance in Binary Maze:" << endl;

    // Optimal approach
    int distOptimal = optimalBFS(grid, n, m, source, destination);
    cout << "Optimal (BFS) Result: " << distOptimal << endl;
    
    // Brute-force approach
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    int distBruteForce = bruteForceDFS(grid, n, m, source.first, source.second, destination.first, destination.second, visited);
    
    if (distBruteForce == numeric_limits<int>::max()) {
        cout << "Brute-Force Result: -1 (No path found)" << endl;
    } else {
        cout << "Brute-Force Result: " << distBruteForce << endl;
    }
    
    return 0;
}

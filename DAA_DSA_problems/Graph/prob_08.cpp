#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <cmath>
#include <functional>
#include <tuple>
#include <algorithm>

using namespace std;

// Brute-force approach using a recursive DFS. This method explores
// all possible paths, leading to a high time complexity.
// Time Complexity: O(4^(N*M)) in the worst case, as it explores all paths.
// Space Complexity: O(N*M) for the recursion stack and visited array.
int minEffortBruteForce(const vector<vector<int>>& heights, int rows, int cols, int r, int c, vector<vector<bool>>& visited) {
    // Base cases for recursion
    if (r < 0 || r >= rows || c < 0 || c >= cols || visited[r][c]) {
        return numeric_limits<int>::max();
    }
    
    if (r == rows - 1 && c == cols - 1) {
        return 0; // The effort to reach the destination from itself is 0
    }
    
    visited[r][c] = true;
    
    int minOverallEffort = numeric_limits<int>::max();
    int delRow[] = {-1, 1, 0, 0};
    int delCol[] = {0, 0, -1, 1};
    
    for (int i = 0; i < 4; ++i) {
        int nextR = r + delRow[i];
        int nextC = c + delCol[i];
        
        if (nextR >= 0 && nextR < rows && nextC >= 0 && nextC < cols && !visited[nextR][nextC]) {
            int currentPathDiff = abs(heights[nextR][nextC] - heights[r][c]);
            int effortFromNeighbor = minEffortBruteForce(heights, rows, cols, nextR, nextC, visited);
            
            if (effortFromNeighbor != numeric_limits<int>::max()) {
                int totalEffort = max(currentPathDiff, effortFromNeighbor);
                minOverallEffort = min(minOverallEffort, totalEffort);
            }
        }
    }
    
    visited[r][c] = false; // Backtrack
    return minOverallEffort;
}

// Optimal approach using Dijkstra's algorithm with a priority queue.
// This is the ideal algorithm for finding the minimum effort path.
// Time Complexity: O(E log V) or O(N*M log(N*M))
// Space Complexity: O(N*M)
int minEffortDijkstra(const vector<vector<int>>& heights) {
    int rows = heights.size();
    int cols = heights[0].size();
    
    // Priority queue stores {effort, row, col}
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;
    
    // Distances array to store the minimum effort to reach each cell
    vector<vector<int>> efforts(rows, vector<int>(cols, numeric_limits<int>::max()));
    
    efforts[0][0] = 0;
    pq.push({0, 0, 0}); // {effort, row, col}
    
    int delRow[] = {-1, 1, 0, 0};
    int delCol[] = {0, 0, -1, 1};
    
    while (!pq.empty()) {
        auto [currentEffort, r, c] = pq.top();
        pq.pop();
        
        if (currentEffort > efforts[r][c]) {
            continue;
        }
        
        if (r == rows - 1 && c == cols - 1) {
            return currentEffort;
        }
        
        for (int i = 0; i < 4; ++i) {
            int nextR = r + delRow[i];
            int nextC = c + delCol[i];
            
            if (nextR >= 0 && nextR < rows && nextC >= 0 && nextC < cols) {
                int newEffort = max(currentEffort, abs(heights[nextR][nextC] - heights[r][c]));
                
                if (newEffort < efforts[nextR][nextC]) {
                    efforts[nextR][nextC] = newEffort;
                    pq.push({newEffort, nextR, nextC});
                }
            }
        }
    }
    
    return -1; // Path not possible
}

int main() {
    vector<vector<int>> heights = {{1, 2, 2}, {3, 8, 2}, {5, 3, 5}};
    int rows = heights.size();
    int cols = heights[0].size();
    
    cout << "Path With Minimum Effort:" << endl;
    
    // Brute-force approach
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    int bruteForceResult = minEffortBruteForce(heights, rows, cols, 0, 0, visited);
    cout << "Brute-Force Result: " << bruteForceResult << endl;
    
    // Optimal approach
    int optimalResult = minEffortDijkstra(heights);
    cout << "Optimal (Dijkstra) Result: " << optimalResult << endl;
    
    return 0;
}

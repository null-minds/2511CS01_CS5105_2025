#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// Brute-force function to find the distance of the nearest 1 for each cell.
// Time Complexity: O(N*M * N*M)
// Space Complexity: O(N*M)
vector<vector<int>> solveBruteForce(const vector<vector<int>>& grid) {
    int rows = grid.size();
    if (rows == 0) return {};
    int cols = grid[0].size();
    
    vector<vector<int>> result(rows, vector<int>(cols, INT_MAX));
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 1) {
                result[i][j] = 0;
                continue;
            }
            
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    if (grid[r][c] == 1) {
                        int dist = abs(i - r) + abs(j - c);
                        result[i][j] = min(result[i][j], dist);
                    }
                }
            }
        }
    }
    
    return result;
}

// Optimal function using Multi-Source BFS.
// Time Complexity: O(N*M)
// Space Complexity: O(N*M)
vector<vector<int>> solveOptimal(const vector<vector<int>>& grid) {
    int rows = grid.size();
    if (rows == 0) return {};
    int cols = grid[0].size();
    
    queue<pair<int, int>> q;
    vector<vector<int>> dist(rows, vector<int>(cols, -1));
    
    // Step 1: Initialize the queue with all cells that contain a 1
    // and set their distance to 0.
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 1) {
                q.push({i, j});
                dist[i][j] = 0;
            }
        }
    }
    
    // Directions for 4-directional movement
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    
    // Step 2: Perform BFS traversal
    while (!q.empty()) {
        pair<int, int> current = q.front();
        q.pop();
        
        int x = current.first;
        int y = current.second;
        
        // Check all 4 neighbors
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            
            // Check if the neighbor is valid and unvisited
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && dist[nx][ny] == -1) {
                dist[nx][ny] = dist[x][y] + 1;
                q.push({nx, ny});
            }
        }
    }
    
    return dist;
}

// Helper function to print the grid
void printGrid(const vector<vector<int>>& grid) {
    for (const auto& row : grid) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

int main() {
    // Example 1: Standard case with multiple 1s
    vector<vector<int>> example1 = {
        {0, 1, 0},
        {0, 0, 0},
        {0, 1, 0}
    };
    
    cout << "Example 1 Input:" << endl;
    printGrid(example1);
    
    cout << "\nBrute Force Output:" << endl;
    vector<vector<int>> resultBrute1 = solveBruteForce(example1);
    printGrid(resultBrute1);
    
    cout << "\nOptimal Output:" << endl;
    vector<vector<int>> resultOptimal1 = solveOptimal(example1);
    printGrid(resultOptimal1);
    
    cout << "\n-------------------\n" << endl;
    
    // Example 2: A simple case with one 1 at the corner
    vector<vector<int>> example2 = {
        {1, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };
    
    cout << "Example 2 Input:" << endl;
    printGrid(example2);
    
    cout << "\nBrute Force Output:" << endl;
    vector<vector<int>> resultBrute2 = solveBruteForce(example2);
    printGrid(resultBrute2);
    
    cout << "\nOptimal Output:" << endl;
    vector<vector<int>> resultOptimal2 = solveOptimal(example2);
    printGrid(resultOptimal2);
    
    return 0;
}

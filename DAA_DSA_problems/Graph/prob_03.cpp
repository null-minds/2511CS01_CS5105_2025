#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Brute-force function to solve the Rotten Oranges problem
int orangesRottingBruteForce(vector<vector<int>>& grid) {
    int rows = grid.size();
    if (rows == 0) return 0;
    int cols = grid[0].size();
    int minutes = 0;
    
    while (true) {
        bool rotSpread = false;
        vector<pair<int, int>> newlyRotten;
        
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (grid[i][j] == 2) {
                    // Check 4-directional neighbors
                    int dx[] = {0, 0, 1, -1};
                    int dy[] = {1, -1, 0, 0};
                    
                    for (int k = 0; k < 4; ++k) {
                        int nx = i + dx[k];
                        int ny = j + dy[k];
                        
                        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && grid[nx][ny] == 1) {
                            newlyRotten.push_back({nx, ny});
                            rotSpread = true;
                        }
                    }
                }
            }
        }
        
        if (!rotSpread) break;
        
        // Mark the newly rotten oranges
        for (const auto& p : newlyRotten) {
            grid[p.first][p.second] = 2;
        }
        
        minutes++;
    }
    
    // Check if any fresh oranges remain
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 1) return -1;
        }
    }
    
    return minutes;
}

// Optimal function using BFS to solve the Rotten Oranges problem
int orangesRottingOptimal(vector<vector<int>>& grid) {
    int rows = grid.size();
    if (rows == 0) return 0;
    int cols = grid[0].size();
    int freshCount = 0;
    queue<pair<int, int>> rottenQueue;
    
    // Step 1: Initialize the queue with all initially rotten oranges and count fresh ones
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 2) {
                rottenQueue.push({i, j});
            } else if (grid[i][j] == 1) {
                freshCount++;
            }
        }
    }
    
    if (freshCount == 0) return 0;
    
    int minutes = 0;
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    
    // Step 2: Perform BFS
    while (!rottenQueue.empty()) {
        int queueSize = rottenQueue.size();
        bool rottedInThisMinute = false;
        
        while (queueSize--) {
            int x = rottenQueue.front().first;
            int y = rottenQueue.front().second;
            rottenQueue.pop();
            
            for (int i = 0; i < 4; ++i) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                
                if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && grid[nx][ny] == 1) {
                    grid[nx][ny] = 2; // Rot the fresh orange
                    rottenQueue.push({nx, ny});
                    freshCount--;
                    rottedInThisMinute = true;
                }
            }
        }
        
        if (rottedInThisMinute) {
            minutes++;
        }
    }
    
    // Step 3: Check if all fresh oranges were rotted
    return freshCount == 0 ? minutes : -1;
}

int main() {
    vector<vector<int>> example1 = { {2, 1, 1}, {0, 1, 1}, {1, 0, 1} };
    vector<vector<int>> example1_copy = example1; // To test both functions on the original input

    cout << "Example 1:" << endl;
    int bruteForceResult = orangesRottingBruteForce(example1);
    cout << "Brute Force Output: " << bruteForceResult << endl;

    int optimalResult = orangesRottingOptimal(example1_copy);
    cout << "Optimal Output: " << optimalResult << endl;

    cout << endl;
    
    vector<vector<int>> example2 = { {2, 1, 1}, {1, 1, 0}, {0, 1, 1} };
    vector<vector<int>> example2_copy = example2;
    
    cout << "Example 2:" << endl;
    int bruteForceResult2 = orangesRottingBruteForce(example2);
    cout << "Brute Force Output: " << bruteForceResult2 << endl;

    int optimalResult2 = orangesRottingOptimal(example2_copy);
    cout << "Optimal Output: " << optimalResult2 << endl;

    return 0;
}

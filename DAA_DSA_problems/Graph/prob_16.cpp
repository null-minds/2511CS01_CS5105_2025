#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Helper function for the brute-force approach, performing a traversal
// to find if a connected component of 'O's touches a boundary.
bool isComponentConnectedToBoundary(int row, int col, int n, int m,
                                    const vector<vector<char>>& mat,
                                    vector<vector<bool>>& visited,
                                    vector<pair<int, int>>& currentComponent) {
    if (row < 0 || row >= n || col < 0 || col >= m || visited[row][col] || mat[row][col] == 'X') {
        return false;
    }

    visited[row][col] = true;
    currentComponent.push_back({row, col});

    if (row == 0 || row == n - 1 || col == 0 || col == m - 1) {
        return true;
    }

    bool touchesBoundary = false;
    int delrow[] = {-1, 0, 1, 0};
    int delcol[] = {0, 1, 0, -1};

    for (int i = 0; i < 4; ++i) {
        if (isComponentConnectedToBoundary(row + delrow[i], col + delcol[i], n, m, mat, visited, currentComponent)) {
            touchesBoundary = true;
        }
    }
    return touchesBoundary;
}

// Brute-force approach: Iterates through every interior 'O' and performs a traversal
// to check if it's connected to a boundary 'O'.
// Time Complexity: O((N*M)^2) in the worst case, as a traversal might be
// performed for every single interior 'O'.
// Space Complexity: O(N*M) for the visited array and recursion stack.
vector<vector<char>> fillBruteForce(int n, int m, vector<vector<char>> mat) {
    if (n <= 2 || m <= 2) {
        return mat;
    }

    vector<vector<char>> result = mat;
    vector<vector<bool>> visited(n, vector<bool>(m, false));

    for (int i = 1; i < n - 1; ++i) {
        for (int j = 1; j < m - 1; ++j) {
            if (result[i][j] == 'O' && !visited[i][j]) {
                vector<pair<int, int>> component;
                bool isConnected = isComponentConnectedToBoundary(i, j, n, m, mat, visited, component);
                if (!isConnected) {
                    for (const auto& p : component) {
                        result[p.first][p.second] = 'X';
                    }
                }
            }
        }
    }
    return result;
}


// Optimal DFS helper function to traverse and mark 'O's connected to the boundary.
// It marks 'O's with a temporary character ('#') to signify they are connected.
void dfsOptimal(int row, int col, int n, int m, vector<vector<char>>& board) {
    if (row < 0 || row >= n || col < 0 || col >= m || board[row][col] != 'O') {
        return;
    }

    board[row][col] = '#';
    int delrow[] = {-1, 0, 1, 0};
    int delcol[] = {0, 1, 0, -1};

    for (int i = 0; i < 4; ++i) {
        dfsOptimal(row + delrow[i], col + delcol[i], n, m, board);
    }
}

// Optimal approach: uses DFS to solve the problem efficiently.
// Time Complexity: O(N*M), as each cell is visited at most once.
// Space Complexity: O(N*M) for the recursion stack.
vector<vector<char>> fillOptimal(int n, int m, vector<vector<char>> mat) {
    if (n == 0 || m == 0) return mat;

    // Mark 'O's on the boundaries and their connected components
    for (int j = 0; j < m; ++j) {
        if (mat[0][j] == 'O') {
            dfsOptimal(0, j, n, m, mat);
        }
        if (mat[n - 1][j] == 'O') {
            dfsOptimal(n - 1, j, n, m, mat);
        }
    }

    for (int i = 0; i < n; ++i) {
        if (mat[i][0] == 'O') {
            dfsOptimal(i, 0, n, m, mat);
        }
        if (mat[i][m - 1] == 'O') {
            dfsOptimal(i, m - 1, n, m, mat);
        }
    }

    // Flip 'O's to 'X' and restore marked cells to 'O'
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (mat[i][j] == 'O') {
                mat[i][j] = 'X';
            } else if (mat[i][j] == '#') {
                mat[i][j] = 'O';
            }
        }
    }
    return mat;
}

int main() {
    vector<vector<char>> mat{
        {'X', 'X', 'X', 'X'},
        {'X', 'O', 'X', 'X'},
        {'X', 'O', 'O', 'X'},
        {'X', 'O', 'X', 'X'},
        {'X', 'X', 'O', 'O'}
    };
    int n = 5, m = 4;

    cout << "Original Matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << mat[i][j] << " ";
        }
        cout << "\n";
    }

    // Brute-Force
    vector<vector<char>> resultBruteForce = fillBruteForce(n, m, mat);
    cout << "\nResult (Brute-Force):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << resultBruteForce[i][j] << " ";
        }
        cout << "\n";
    }

    // Optimal
    vector<vector<char>> resultOptimal = fillOptimal(n, m, mat);
    cout << "\nResult (Optimal):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << resultOptimal[i][j] << " ";
        }
        cout << "\n";
    }

    return 0;
}

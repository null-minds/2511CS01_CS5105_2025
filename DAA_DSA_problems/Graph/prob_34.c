#include <bits/stdc++.h>
using namespace std;

void floydWarshall(vector<vector<int>>& matrix) {
    int n = matrix.size();
    const int INF = 1e9; // A large number to denote infinity
    
    // Step 1: Replace -1 with INF (except diagonal)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == -1 && i != j) {
                matrix[i][j] = INF;
            }
        }
    }

    // Step 2: Core Floyd-Warshall Algorithm
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][k] < INF && matrix[k][j] < INF) {
                    matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);
                }
            }
        }
    }

    // Step 3: Convert INF back to -1
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == INF) {
                matrix[i][j] = -1;
            }
        }
    }
}

int main() {
    int n;
    cout << "Enter number of vertices: ";
    cin >> n;

    vector<vector<int>> matrix(n, vector<int>(n));
    cout << "Enter adjacency matrix (use -1 if no edge exists):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> matrix[i][j];
        }
    }

    floydWarshall(matrix);

    cout << "\n Shortest distance matrix after Floyd-Warshall:\n";
    for (auto row : matrix) {
        for (auto val : row) {
            cout << val << " ";
        }
        cout << "\n";
    }
    return 0;
}

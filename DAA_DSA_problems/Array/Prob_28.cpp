#include <bits/stdc++.h>
using namespace std;

// Brute Force: simulate movement and mark visited
class BruteForceSolution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        vector<int> res;
        vector<vector<bool>> vis(m, vector<bool>(n, false));
        int dirs[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};
        int dir = 0, r = 0, c = 0;
        for (int i = 0; i < m * n; ++i) {
            res.push_back(matrix[r][c]);
            vis[r][c] = true;
            int nr = r + dirs[dir][0], nc = c + dirs[dir][1];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n || vis[nr][nc]) {
                dir = (dir + 1) % 4;
                nr = r + dirs[dir][0];
                nc = c + dirs[dir][1];
            }
            r = nr; c = nc;
        }
        return res;
    }
};

// Optimal: use boundary tracking (top, bottom, left, right)
class OptimalSolution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        int top = 0, left = 0;
        int bottom = matrix.size() - 1, right = matrix[0].size() - 1;
        vector<int> res;
        while (top <= bottom && left <= right) {
            for (int j = left; j <= right; ++j) res.push_back(matrix[top][j]);
            top++;
            for (int i = top; i <= bottom; ++i) res.push_back(matrix[i][right]);
            right--;
            if (top <= bottom)
                for (int j = right; j >= left; --j) res.push_back(matrix[bottom][j]);
            bottom--;
            if (left <= right)
                for (int i = bottom; i >= top; --i) res.push_back(matrix[i][left]);
            left++;
        }
        return res;
    }
};

int main() {
    int m, n;
    cin >> m >> n;
    vector<vector<int>> matrix(m, vector<int>(n));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            cin >> matrix[i][j];

    OptimalSolution opt;
    vector<int> res = opt.spiralOrder(matrix);
    for (int x : res) cout << x << " ";
    cout << endl;
    return 0;
}

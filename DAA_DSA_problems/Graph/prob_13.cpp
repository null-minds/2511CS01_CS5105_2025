#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Brute-force function using iterative passes to solve the Flood Fill problem.
// Time Complexity: O(k * M * N), where k is the number of passes.
// Space Complexity: O(M * N) for the copy and queue.
vector<vector<int>> floodFillBruteForce(vector<vector<int>>& image, int sr, int sc, int newColor) {
    int rows = image.size();
    if (rows == 0) return {};
    int cols = image[0].size();
    int initialColor = image[sr][sc];
    
    if (initialColor == newColor) {
        return image;
    }
    
    vector<vector<int>> newImage = image;
    queue<pair<int, int>> q;
    
    q.push({sr, sc});
    newImage[sr][sc] = newColor;

    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};
    
    while (!q.empty()) {
        pair<int, int> current = q.front();
        q.pop();
        
        for (int i = 0; i < 4; ++i) {
            int next_x = current.first + dx[i];
            int next_y = current.second + dy[i];
            
            if (next_x >= 0 && next_x < rows && next_y >= 0 && next_y < cols && 
                newImage[next_x][next_y] == initialColor) {
                
                newImage[next_x][next_y] = newColor;
                q.push({next_x, next_y});
            }
        }
    }
    
    return newImage;
}

// Helper function for the optimal DFS approach.
void dfsFill(vector<vector<int>>& image, int r, int c, int newColor, int initialColor) {
    int rows = image.size();
    int cols = image[0].size();
    
    // Check for base cases: out of bounds or color mismatch
    if (r < 0 || r >= rows || c < 0 || c >= cols || image[r][c] != initialColor) {
        return;
    }
    
    // Fill the current pixel
    image[r][c] = newColor;
    
    // Recursively fill neighbors
    dfsFill(image, r + 1, c, newColor, initialColor);
    dfsFill(image, r - 1, c, newColor, initialColor);
    dfsFill(image, r, c + 1, newColor, initialColor);
    dfsFill(image, r, c - 1, newColor, initialColor);
}

// Optimal function using a recursive DFS approach.
// Time Complexity: O(M * N)
// Space Complexity: O(M * N) for the recursion stack in the worst case.
vector<vector<int>> floodFillOptimal(vector<vector<int>>& image, int sr, int sc, int newColor) {
    int initialColor = image[sr][sc];
    
    // Avoid infinite recursion if the colors are the same
    if (initialColor == newColor) {
        return image;
    }
    
    dfsFill(image, sr, sc, newColor, initialColor);
    return image;
}

// Helper function to print the image
void printImage(const vector<vector<int>>& image) {
    for (const auto& row : image) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

int main() {
    vector<vector<int>> image1 = {
        {1, 1, 1},
        {1, 1, 0},
        {1, 0, 1}
    };
    int startRow1 = 1, startCol1 = 1, newColor1 = 2;
    
    vector<vector<int>> image2 = image1;
    
    cout << "Original Image:" << endl;
    printImage(image1);
    
    // Test Brute-Force
    vector<vector<int>> resultBruteForce = floodFillBruteForce(image1, startRow1, startCol1, newColor1);
    cout << "\nResult after Brute-Force:" << endl;
    printImage(resultBruteForce);
    
    // Test Optimal
    vector<vector<int>> resultOptimal = floodFillOptimal(image2, startRow1, startCol1, newColor1);
    cout << "\nResult after Optimal (DFS):" << endl;
    printImage(resultOptimal);
    
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

// -------- Brute Force (DFS Cycle Detection) --------
bool dfsCycle(int node, vector<vector<int>>& adj, vector<int>& visited) {
    if (visited[node] == 1) return true;  // cycle
    if (visited[node] == 2) return false; // already checked
    
    visited[node] = 1; // mark as visiting
    
    for (int neigh : adj[node]) {
        if (dfsCycle(neigh, adj, visited)) return true;
    }
    
    visited[node] = 2; // mark as processed
    return false;
}

bool canFinishBruteForce(int N, vector<pair<int,int>>& prerequisites) {
    vector<vector<int>> adj(N);
    for (auto& p : prerequisites) {
        adj[p.second].push_back(p.first);
    }
    
    vector<int> visited(N, 0);
    for (int i = 0; i < N; i++) {
        if (visited[i] == 0) {
            if (dfsCycle(i, adj, visited)) return false;
        }
    }
    return true;
}

// -------- Optimal (Kahn’s Algorithm) --------
vector<int> findOrderOptimal(int N, vector<pair<int,int>>& prerequisites) {
    vector<vector<int>> adj(N);
    vector<int> indegree(N, 0);
    
    for (auto& p : prerequisites) {
        adj[p.second].push_back(p.first);
        indegree[p.first]++;
    }
    
    queue<int> q;
    for (int i = 0; i < N; i++) {
        if (indegree[i] == 0) q.push(i);
    }
    
    vector<int> order;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        order.push_back(node);
        
        for (int neigh : adj[node]) {
            indegree[neigh]--;
            if (indegree[neigh] == 0) q.push(neigh);
        }
    }
    
    if ((int)order.size() == N) return order;
    return {}; // empty if cycle
}

// -------- Driver Code --------
int main() {
    int N = 4;
    vector<pair<int,int>> prerequisites1 = {{1,0},{2,1},{3,2}};
    vector<pair<int,int>> prerequisites2 = {{1,2},{3,4},{2,4},{4,1}};
    
    cout << "Brute Force (Cycle detection):\n";
    cout << (canFinishBruteForce(N, prerequisites1) ? "Yes" : "No") << endl;
    cout << (canFinishBruteForce(N, prerequisites2) ? "Yes" : "No") << endl;
    
    cout << "\nOptimal (Topological Ordering):\n";
    vector<int> order = findOrderOptimal(N, prerequisites1);
    if (!order.empty()) {
        for (int x : order) cout << x << " ";
        cout << endl;
    } else {
        cout << "Impossible" << endl;
    }
    
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

// 🔹 Disjoint Set (Union-Find)
class DisjointSet {
    vector<int> parent, rank;
public:
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    int findParent(int u) {
        if (parent[u] == u) return u;
        return parent[u] = findParent(parent[u]);
    }

    void unionByRank(int u, int v) {
        int pu = findParent(u);
        int pv = findParent(v);
        if (pu == pv) return;

        if (rank[pu] < rank[pv]) parent[pu] = pv;
        else if (rank[pv] < rank[pu]) parent[pv] = pu;
        else {
            parent[pv] = pu;
            rank[pu]++;
        }
    }
};

// 🔹 Brute Force Approach
vector<vector<string>> accountsMergeBruteForce(vector<vector<string>>& accounts) {
    int n = accounts.size();
    vector<set<string>> emailSets(n);

    for (int i = 0; i < n; i++) {
        for (int j = 1; j < accounts[i].size(); j++) {
            emailSets[i].insert(accounts[i][j]);
        }
    }

    bool merged = true;
    while (merged) {
        merged = false;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                vector<string> intersection;
                set_intersection(emailSets[i].begin(), emailSets[i].end(),
                                 emailSets[j].begin(), emailSets[j].end(),
                                 back_inserter(intersection));
                if (!intersection.empty()) {
                    emailSets[i].insert(emailSets[j].begin(), emailSets[j].end());
                    emailSets[j].clear();
                    merged = true;
                }
            }
        }
    }

    vector<vector<string>> res;
    for (int i = 0; i < n; i++) {
        if (!emailSets[i].empty()) {
            vector<string> account;
            account.push_back(accounts[i][0]); // Name
            account.insert(account.end(), emailSets[i].begin(), emailSets[i].end());
            res.push_back(account);
        }
    }
    return res;
}

// 🔹 Optimal Approach using Disjoint Set
vector<vector<string>> accountsMergeOptimal(vector<vector<string>>& accounts) {
    int n = accounts.size();
    DisjointSet ds(n);
    unordered_map<string, int> emailToIndex;

    for (int i = 0; i < n; i++) {
        for (int j = 1; j < accounts[i].size(); j++) {
            string email = accounts[i][j];
            if (emailToIndex.count(email)) {
                ds.unionByRank(i, emailToIndex[email]);
            } else {
                emailToIndex[email] = i;
            }
        }
    }

    unordered_map<int, set<string>> mergedEmails;
    for (auto &it : emailToIndex) {
        string email = it.first;
        int parent = ds.findParent(it.second);
        mergedEmails[parent].insert(email);
    }

    vector<vector<string>> res;
    for (auto &it : mergedEmails) {
        vector<string> account;
        account.push_back(accounts[it.first][0]); // Name
        account.insert(account.end(), it.second.begin(), it.second.end());
        res.push_back(account);
    }

    return res;
}

int main() {
    vector<vector<string>> accounts = {
        {"John","johnsmith@mail.com","john_newyork@mail.com"},
        {"John","johnsmith@mail.com","john00@mail.com"},
        {"Mary","mary@mail.com"},
        {"John","johnnybravo@mail.com"}
    };

    cout << "Brute Force Output:\n";
    auto brute = accountsMergeBruteForce(accounts);
    for (auto &acc : brute) {
        cout << "[ ";
        for (auto &s : acc) cout << s << " ";
        cout << "]\n";
    }

    cout << "\nOptimal Output:\n";
    auto optimal = accountsMergeOptimal(accounts);
    for (auto &acc : optimal) {
        cout << "[ ";
        for (auto &s : acc) cout << s << " ";
        cout << "]\n";
    }

    return 0;
}


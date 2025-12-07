#include <bits/stdc++.h>
using namespace std;

// Brute Force: Compare every pair and merge overlaps iteratively
class BruteForceSolution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        int n = intervals.size();
        for (auto &v : intervals)
            if (v[0] > v[1]) swap(v[0], v[1]);
        bool merged = true;
        while (merged) {
            merged = false;
            for (int i = 0; i < intervals.size(); ++i) {
                for (int j = i + 1; j < intervals.size(); ++j) {
                    if (!(intervals[i][1] < intervals[j][0] || intervals[j][1] < intervals[i][0])) {
                        intervals[i][0] = min(intervals[i][0], intervals[j][0]);
                        intervals[i][1] = max(intervals[i][1], intervals[j][1]);
                        intervals.erase(intervals.begin() + j);
                        merged = true;
                        break;
                    }
                }
                if (merged) break;
            }
        }
        sort(intervals.begin(), intervals.end());
        return intervals;
    }
};

// Optimal: Sort and merge in one pass
class OptimalSolution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end());
        vector<vector<int>> res;
        for (auto &it : intervals) {
            if (res.empty() || res.back()[1] < it[0])
                res.push_back(it);
            else
                res.back()[1] = max(res.back()[1], it[1]);
        }
        return res;
    }
};

int main() {
    int n;
    cin >> n;
    vector<vector<int>> intervals(n, vector<int>(2));
    for (int i = 0; i < n; ++i)
        cin >> intervals[i][0] >> intervals[i][1];

    OptimalSolution opt;
    vector<vector<int>> ans = opt.merge(intervals);
    for (auto &v : ans)
        cout << "[" << v[0] << "," << v[1] << "] ";
    cout << endl;
    return 0;
}

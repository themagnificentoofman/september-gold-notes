// Source: https://usaco.guide/general/io

#include <bits/stdc++.h>
using namespace std;

int main() {
    cin.tie(0)->sync_with_stdio(0);

    int n;
    cin >> n;
    vector<string> names(n);
    for (int i = 0; i < n; i++) cin >> names[i];

    // adjacency list for 26 letters
    vector<vector<int>> adj(26);
    vector<int> indeg(26, 0);
    vector<bool> used(26, false);

    // mark used letters
    for (auto &s : names)
        for (char c : s) used[c - 'a'] = true;

    // build constraints
    for (int i = 0; i < n - 1; i++) {
        string &a = names[i], &b = names[i + 1];
        int len = min(a.size(), b.size());
        bool found = false;
        for (int j = 0; j < len; j++) {
            if (a[j] != b[j]) {
                int u = a[j] - 'a', v = b[j] - 'a';
                adj[u].push_back(v);
                found = true;
                break;
            }
        }
        if (!found && a.size() > b.size()) {
            cout << "Impossible\n";
            return 0;
        }
    }

    // compute indegrees
    for (int u = 0; u < 26; u++) {
        for (int v : adj[u]) indeg[v]++;
    }

    // topological sort (Kahn's algorithm)
    queue<int> q;
    for (int i = 0; i < 26; i++)
        if (indeg[i] == 0) q.push(i);

    string order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back('a' + u);
        for (int v : adj[u]) {
            if (--indeg[v] == 0) q.push(v);
        }
    }

    if ((int)order.size() != 26) {
        cout << "Impossible\n";
        return 0;
    }

    cout << order << "\n";

}
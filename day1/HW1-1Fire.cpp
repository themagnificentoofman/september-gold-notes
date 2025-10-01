// Source: https://usaco.guide/general/io

#include <bits/stdc++.h>
using namespace std;

struct Cell { int r, c, t; };

int dr[]{1, -1, 0, 0}, dc[]{0, 0, 1, -1};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<string> maze(n);
    for (int i = 0; i < n; i++) cin >> maze[i];

    queue<Cell> fireQ;
    vector<vector<int>> fireTime(n, vector<int>(m, INT_MAX));
    int jr, jc; // Joe pos

    // Initialize positions
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (maze[i][j] == 'F') {
                fireQ.push({i, j, 0});
                fireTime[i][j] = 0;
            }
            if (maze[i][j] == 'J') {
                jr = i; jc = j;
            }
        }
    }

    // BFS for fire spread
    while (!fireQ.empty()) {
        const Cell &cur = fireQ.front();
        int r = cur.r, c = cur.c, t = cur.t;
        fireQ.pop();
        // explore all adjacent cells
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k], nt = t + 1;
            if (nr >= 0 && nr < n && nc >= 0 && nc < m) {
                if (maze[nr][nc] != '#' && nt < fireTime[nr][nc]) {
                    fireTime[nr][nc] = nt;
                    fireQ.push({nr, nc, nt});
                }
            }
        }
    }

    // BFS for Joe
    queue<Cell> joeQ;
    vector<vector<bool>> vis(n, vector<bool>(m, false));
    joeQ.push({jr, jc, 0});
    vis[jr][jc] = 1;
    int escapeTime = -1;
    while (!joeQ.empty()) {
        const Cell &cur = joeQ.front();
        int r = cur.r, c = cur.c, t = cur.t;
        joeQ.pop();
        // If Joe is at the boundary, he escapes
        if (r == 0 || r == n-1 || c == 0 || c == m-1) {
            escapeTime = t + 1; // time to step out
            break;
        }
        // explore all adjacent cells
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k], nt = t + 1;
            if (nr >= 0 && nr < n && nc >= 0 && nc < m) {
                if (!vis[nr][nc] && maze[nr][nc] != '#') {
                    if (nt < fireTime[nr][nc]) {
                        vis[nr][nc] = 1;
                        joeQ.push({nr, nc, nt});
                    }
                }
            }
        }
    }

    if (escapeTime == -1) cout << "IMPOSSIBLE\n";
    else cout << escapeTime << "\n";
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    int t; cin >> t;
    while (t--) solve();
}

// Source: https://usaco.guide/general/io
// #pragma GCC optimize("Ofast")

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using db = long double;
template<class T> using V = vector<T>;
template<class T1, class T2> using P = pair<T1, T2>;
#define bg(x) begin(x)
#define all(x) bg(x), end(x)
#define sz(x) (int)(x).size()
#define pb push_back
#define f first
#define s second
using vi = V<int>;
using pi = P<int,int>;
inline namespace FileIO {
	void setIn(string s){freopen(s.c_str(),"r",stdin);}
	void setOut(string s){freopen(s.c_str(),"w",stdout);}
	void setIO(string s = ""){
		cin.tie(0)->sync_with_stdio(0);
		cin.exceptions(cin.failbit);
		if (sz(s)) setIn(s+".in"), setOut(s+".out");
	}
}
const db PI = acos((db)-1);

int vis[205];
vi adj[205];

inline bool dfs(int u, int col) {
    vis[u] = col;
    for(const int &v : adj[u]) {
        if(vis[v] == -1) /* recurse only if unvisited */ {
            if (!dfs(v, 1 - col)) return false;
        } else if(vis[v] == col) /* conflict */ {
            return false;
        }
    }
    return true;
}

int main() {
	setIO();
    int n;
    while(cin >> n && n) {
        int m; cin >> m;
        for(int i = 0; i < n; i++) {
            adj[i].clear();
            adj[i].shrink_to_fit();
        }
        for(int i = 0; i < n; i++) vis[i] = -1;
        for(int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].pb(v);
            adj[v].pb(u);
        }
        cout << (dfs(0, 0) ? "BICOLORABLE." : "NOT BICOLORABLE.") << '\n';
    }
}
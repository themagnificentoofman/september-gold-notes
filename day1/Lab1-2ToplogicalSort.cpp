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

vi vis, indeg;
V<vi> adj;

int main() {
	setIO();
    int n, m; cin >> n >> m;
    vi ans;
    adj.resize(n);
    vis.resize(n);
    indeg.resize(n);
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].pb(v);
        indeg[v]++;
    }
    queue<int> q;
    for(int i = 0; i < n; i++) {
        if(indeg[i] == 0) q.push(i);
    }
    while(!q.empty()) {
        auto u = q.front();
        q.pop();
        vis[u] = 1;
        ans.pb(u);
        for(const int &v : adj[u]) {
            if(!vis[v]) {
                if(--indeg[v] == 0) q.push(v);
            }
        }
    }
    for(int i = 0; i < sz(ans); i++) {
        cout << ans[i] << " \n"[i == sz(ans) - 1];
    }
}
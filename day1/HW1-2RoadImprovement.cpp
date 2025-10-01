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

/*
 Problem:
 - We have a tree with n nodes and n-1 edges (roads).
 - Each day, we can repair a set of edges such that no two chosen edges share a node.
 - Find the minimum number of days and a schedule.

 Key Facts:
 - A "day" corresponds to a matching.
 - The minimum number of matchings needed to cover all edges in a tree equals
   the maximum degree Δ.
 - So answer k = Δ. We just need to produce an edge-coloring with Δ colors.

 Construction:
 - Root the tree (we pick a node with maximum degree to guarantee all colors appear). <- IMPORTANT !!!
 - BFS from the root. For each node v, assign colors to its incident (uncolored) edges:
     * Use colors 1..Δ but skip the color used by the parent edge (if any).
     * Since deg(v) <= Δ and we avoid at most one color (the parent's), there are enough colors.
 - Collect edges by color: edges with color c are repaired on day c.

 Complexity:
 - O(n) time and memory.
 */

int main() {
	setIO();
    int n;
    cin >> n;
    int m = n - 1;
    V<pi> edges(m + 1);
    V<V<pi>> adj(n + 1); // adj[u] -> {v, edge_id}
    for(int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        edges[i] = {u, v};
        adj[u].pb({v, i});
        adj[v].pb({u, i});
    }
    int root, mxdeg = 0;
    for(int v = 1; v <= n; v++) {
        if(sz(adj[v]) > mxdeg) {
            mxdeg = sz(adj[v]);
            root = v;
        }
    }
    vi col(m + 1); // keep track of colors for edges to parent
    queue<pi> q; // -> {node, color of edge to parent}
    q.push({root, 0}); // since the root doesn't have a parent, its "edge to parent" doesn't exist, and therefore has no color (color 0)
    while(!q.empty()) {
        auto [v, pcol] = q.front(); q.pop(); // copy by VALUE before popping (whoops)
        int curcol = 1; // iterate through 1...mxdeg, skipping pcol
        for(const auto &[to, id] : adj[v]) {
            if(col[id] != 0) continue; // already colored this edge
            if(curcol == pcol) curcol++; // don't clash with parent edge color
            col[id] = curcol;
            q.push({to, curcol});
            curcol++;
            // Note: We don't need to wrap colors because at v we color at most deg(v)-1 edges here,
            // and we have mxdeg colors excluding at most one (pcol), which suffices.
            // deg(v) <= mxdeg
        }
    }
    V<vi> output(mxdeg + 1);
    for(int i = 1; i <= m; i++) {
        output[col[i]].pb(i);
    }
    // output
    cout << mxdeg << '\n';
    for(int i = 1; i <= mxdeg; i++) {
        cout << sz(output[i]);
        for(const int &id : output[i]) cout << ' ' << id;
        cout << '\n';
    }
}
/* Source: https://usaco.guide/general/io */

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
#define fi first
#define se second
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

struct Edge {
    int u, v;
    int w; // years delta (can be negative)
};

int main() {
    setIO();

    int c; 
    if (!(cin >> c)) return 0;
    while (c--) {
        int n, m;
        cin >> n >> m;

        V<Edge> edges; 
        edges.reserve(m);

        for (int i = 0; i < m; ++i) {
            int x, y, t;
            cin >> x >> y >> t;
            edges.pb({x, y, t});
        }

        const ll INF = (ll)4e18;
        V<ll> dist(n, INF);
        dist[0] = 0; // start from our solar system (node 0)

        /*
         Bellman-Ford: relax edges up to n-1 times.
        */
        bool updated = false;
        for (int it = 0; it < n - 1; ++it) {
            updated = false;
            for (const auto& e : edges) {
                if (dist[e.u] == INF) continue;
                ll nd = dist[e.u] + (ll)e.w;
                if (nd < dist[e.v]) {
                    dist[e.v] = nd;
                    updated = true;
                }
            }
            if (!updated) break; // early stop if no changes
        }

        /*
         One more pass: if any edge can still be relaxed, there is a reachable negative cycle.
        */
        bool hasNeg = false;
        for (const auto& e : edges) {
            if (dist[e.u] == INF) continue;
            if (dist[e.u] + (ll)e.w < dist[e.v]) {
                hasNeg = true;
                break;
            }
        }

        cout << (hasNeg ? "possible" : "not possible") << '\n';
    }
}
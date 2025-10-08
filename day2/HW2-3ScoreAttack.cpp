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

constexpr ll INF = 0x3f3f3f3f3f3f3f3f;

struct Edge { int u, v, w; };

int n, m;

int main() {
	setIO();
	cin >> n >> m;
	V<Edge> edges; edges.reserve(m);
	V<vi> radj(n); // reversed graph: for reachability to sink.

	for (int i = 0; i < m; ++i) {
		int x, y, t;
		cin >> x >> y >> t;
		--x; --y;
		edges.pb({x, y, t});
		radj[y].pb(x); // reverse edge for reachability from sink
	}

	// Find all nodes that can reach sink (n-1) in the original graph.
	V<char> canReachSink(n, false);
	deque<int> dq;
	canReachSink[n-1] = true;
	dq.pb(n-1);
	while (!dq.empty()) {
		int x = dq.front(); dq.pop_front();
		for (int p : radj[x]) if (!canReachSink[p]) {
			canReachSink[p] = true;
			dq.pb(p);
		}
	}

	// Bellman-Ford for maximum path sum (initialize to -INF).
	V<ll> dist(n, -INF);
	dist[0] = 0;

	for (int it = 0; it < n - 1; ++it) {
		bool updated = false;
		for (const auto &e : edges) {
			if (dist[e.u] == -INF) continue;
			ll nd = dist[e.u] + (ll)e.w;
			if (nd > dist[e.v]) {
				dist[e.v] = nd;
				updated = true;
			}
		}
		if (!updated) break;
	}

	// Detect a positive cycle that affects the answer (i.e., can reach sink).
	for (const auto &e : edges) {
		if (dist[e.u] == -INF) continue;
		if (canReachSink[e.u] && canReachSink[e.v] && dist[e.u] + (ll)e.w > dist[e.v]) {
			cout << "inf";
			return 0;
		}
	}

	// Unreachable sink
	// if (dist[n-1] == -INF) { /* print spec-required value */ }
	cout << dist[n - 1];
}
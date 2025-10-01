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

vi indeg;
V<vi> adj;

int main() {
	setIO();
	int n, m;
	cin >> n >> m;

	adj.assign(n+1, vi());
	indeg.assign(n+1, 0);

	for (int i = 0; i < m; i++) {
		int u, v; cin >> u >> v; // vertices are 1-based
		adj[u].pb(v);
		indeg[v]++;
	}
	vi p(n);
	for (int &x : p) cin >> x;

	// First pass: walk along p and find the rightmost position where we could pick
	// something strictly larger (still valid) than p[i].
	vi deg = indeg;
	set<int> avail;
	for (int i = 1; i <= n; i++) if (deg[i] == 0) avail.insert(i);

	int pivot = -1;
	for (int i = 0; i < n; i++) {
		// If there exists an available node > p[i], record this as a possible pivot.
		auto it = avail.upper_bound(p[i]);
		if (it != end(avail)) pivot = i;

		// Consume p[i] (p is guaranteed to be a topological order).
		if (!avail.count(p[i])) { cout << -1 << '\n'; return 0; } // safety
		avail.erase(p[i]);
		for (int v : adj[p[i]]) if (--deg[v] == 0) avail.insert(v);
	}

	if (pivot == -1) { cout << -1 << '\n'; return 0; }

	// Second pass (rebuild answer):
	// keep prefix [0..pivot-1] the same
	// at 'pivot' take the smallest available element > p[pivot]
	// then finish greedily with the smallest available each step.
	deg = indeg;
	avail.clear();
	for (int i = 1; i <= n; i++) if (deg[i] == 0) avail.insert(i);

	vi ans; ans.reserve(n);

	// keep prefix
	for (int i = 0; i < pivot; i++) {
		int x = p[i];
		ans.pb(x);
		avail.erase(x);
		for (const int &v : adj[x]) if (--deg[v] == 0) avail.insert(v);
	}

	// replace at pivot
	{
		auto it = avail.upper_bound(p[pivot]);
		int y = *it; // smallest available > p[pivot]
		ans.pb(y);
		avail.erase(y);
		for (const int &v : adj[y]) if (--deg[v] == 0) avail.insert(v);
	}

	// finish with lexicographically smallest continuation
	for (int i = pivot+1; i < n; i++) {
		int x = *bg(avail);
		ans.pb(x);
		avail.erase(bg(avail));
		for (const int &v : adj[x]) if (--deg[v] == 0) avail.insert(v);
	}

	for (int i = 0; i < n; i++) cout << ans[i] << " \n"[i == n-1];
	return 0;
}
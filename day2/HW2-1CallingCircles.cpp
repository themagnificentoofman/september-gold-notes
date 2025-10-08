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

int main() {
	setIO();

	int n, m;
	int ds = 0;

	while (cin >> n >> m) {
		if (n == 0 && m == 0) break;

		unordered_map<string, int> id;
		V<string> name; name.reserve(n);

		auto getId = [&](const string& s)->int {
			auto it = id.find(s);
			if (it != id.end()) return it->se;
			int idx = sz(name);
			id[s] = idx;
			name.pb(s);
			return idx;
		};

		V<pi> edges; edges.reserve(m);
		for (int i = 0; i < m; ++i) {
			string a, b; cin >> a >> b;
			int u = getId(a), v = getId(b);
			edges.pb({u, v});
		}

		int N = sz(name);
		V<V<char>> reach(N, V<char>(N));

		// Self reachability so singletons form their own circles.
		for (int i = 0; i < N; ++i) reach[i][i] = 1;

		// Direct edges.
		for (auto &e : edges) reach[e.fi][e.se] = 1;

		// Floyd–Warshall transitive closure.
		for (int k = 0; k < N; ++k)
			for (int i = 0; i < N; ++i)
				if (reach[i][k]) // tiny speed win
					for (int j = 0; j < N; ++j)
						if (reach[k][j]) reach[i][j] = 1;

		if (ds) cout << "\n";
		cout << "Calling circles for data set " << (++ds) << ":\n";

		vi used(N);
		for (int i = 0; i < N; ++i) if (!used[i]) {
			V<int> comp;
			for (int j = 0; j < N; ++j)
				if (!used[j] && reach[i][j] && reach[j][i]) {
					used[j] = 1;
					comp.pb(j);
				}
			// Print the component.
			for (int t = 0; t < sz(comp); ++t) {
				if (t) cout << ", ";
				cout << name[comp[t]];
			}
			cout << "\n";
		}
	}
}
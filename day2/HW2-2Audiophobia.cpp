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

constexpr int INF = 1e9;

int main() {
	setIO();

	int C, S, Q;
	int tc = 0;

	while (true) {
		cin >> C >> S >> Q;
		if (C == 0 && S == 0 && Q == 0) break;

		++tc;

		/* Initialize dp with INF; diagonal = 0. */
		V<V<int>> dp(C+1, V<int>(C+1, INF));
		for (int i = 1; i <= C; ++i) dp[i][i] = 0;

		/* Read streets; keep the smallest noise if multiple edges exist. */
		for (int i = 0; i < S; ++i) {
			int a, b, d; cin >> a >> b >> d;
			dp[a][b] = min(dp[a][b], d);
			dp[b][a] = min(dp[b][a], d);
		}

		/* Floyd minimax relaxation. */
		for (int k = 1; k <= C; ++k) {
			for (int i = 1; i <= C; ++i) {
				if (dp[i][k] == INF) continue; // small pruning.
				for (int j = 1; j <= C; ++j) {
					int through_k = max(dp[i][k], dp[k][j]);
					dp[i][j] = min(dp[i][j], through_k);
				}
			}
		}

		/* Output formatting: blank line between consecutive cases. */
		if (tc > 1) cout << "\n";
		cout << "Case #" << tc << "\n";

		while (Q--) {
			int u, v; cin >> u >> v;
			if (dp[u][v] == INF) {
				cout << "no path\n";
			} else {
				cout << dp[u][v] << "\n";
			}
		}
	}
}
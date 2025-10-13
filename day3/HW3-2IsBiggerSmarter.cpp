// Source: https://usaco.guide/general/io
// #pragma GCC optimize("Ofast")
// #pragma GCC target("avx2")

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
		// cin.exceptions(cin.failbit); // comment out if breaks things
		if (sz(s)) setIn(s+".in"), setOut(s+".out");
	}
}
const db PI = acos((db)-1);

/*
  Problem: Given (weight W, IQ S) pairs (<=1000 elephants),
  find the longest sequence with strictly increasing W and strictly decreasing S.
  
  Approach:
  1) Read all elephants with their original 1-based index.
  2) Sort by W ascending, and for tie W, by S descending.
  3) O(n^2) DP: dp[i] = best length ending at i, with prev[i] for reconstruction.
     Transition allowed iff W[j] < W[i] and S[j] > S[i].
  4) Reconstruct and print the sequence of original indices.
*/

struct Ele {
	int w, s, id;
};

int main() {
	setIO();
    V<Ele> a;
	int idx = 0;

	// Robust read-until-EOF even with cin exceptions enabled in template.
	while (true) {
		int w, s;
		try {
			if (!(cin >> w >> s)) break;
		} catch (const ios_base::failure&) {
			break; // EOF reached.
		}
		a.pb({w, s, ++idx});
	}

	if (a.empty()) {
		cout << 0 << '\n';
		return 0;
	}

	// Sort by weight asc, IQ desc for stable DP behavior on ties.
	sort(all(a), [](const Ele& x, const Ele& y){
		if (x.w != y.w) return x.w < y.w;
		return x.s > y.s;
	});

	int n = sz(a);
	vi dp(n, 1), pre(n, -1);

	// O(n^2) DP with strict inequalities on both dimensions.
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < i; ++j) {
			if (a[j].w < a[i].w && a[j].s > a[i].s && dp[j] + 1 > dp[i]) {
				dp[i] = dp[j] + 1;
				pre[i] = j;
			}
		}
	}

	// Find best ending position.
	int best = max_element(all(dp)) - bg(dp);

	// Reconstruct sequence of original indices.
	vi seq;
	for (int cur = best; cur != -1; cur = pre[cur]) seq.pb(a[cur].id);
	reverse(all(seq));

	// Output result.
	cout << sz(seq) << '\n';
	for (int id : seq) cout << id << '\n';
}
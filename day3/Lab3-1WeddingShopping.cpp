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
		cin.exceptions(cin.failbit); // comment out if breaks things
		if (sz(s)) setIn(s+".in"), setOut(s+".out");
	}
}
const db PI = acos((db)-1);

inline void solve() {
    int M, C;
    cin >> M >> C;

    // dp[m] == 1 if it's possible to have exactly m money left after current garments.
    vi dp(M + 1, 0), ndp(M + 1, 0);
    dp[M] = 1;

    for (int g = 0; g < C; ++g) {
      int K; cin >> K;
      ndp.assign(M + 1, 0);

      for (int j = 0; j < K; ++j) {
        int price; cin >> price;
        for (int m = 0; m <= M; ++m) {
          if (dp[m] && m - price >= 0) ndp[m - price] = 1;
        }
      }
      dp.swap(ndp);
    }

    int ans = -1;
    for (int m = 0; m <= M; ++m) if (dp[m]) { ans = M - m; break; }

    if (ans < 0) cout << "no solution\n";
    else cout << ans << '\n';
}

int main() {
	setIO();
    int t = 1; cin >> t; // (comment out if one test case)
    while(t--) solve();
}
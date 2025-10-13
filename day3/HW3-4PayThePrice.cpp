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
  Problem:
  Count the number of ways to pay N dollars using coin denominations 1..300 (unlimited supply),
  optionally constrained by the number of coins used:
    - input: N
      -> total number of ways to pay N (any number of coins).
    - input: N L1
      -> ways using at most L1 coins.
    - input: N L1 L2
      -> ways using between L1 and L2 coins (inclusive).

  Observations:
  - Since all coin values are >= 1, the maximum useful coin count for sum N is N.
  - The answer equals the number of integer partitions of N into exactly k parts (sum over k as required).
  - Precompute DP once:
      dp[s][k] = number of ways to make sum s using exactly k coins from {1..300} (order-independent).
    Transition over coin values c = 1..300:
      for s from c..300:
        for k from 1..300:
          dp[s][k] += dp[s - c][k - 1];
    Base: dp[0][0] = 1.
  - Then per query sum dp[N][k] over the requested k-range.
  - Use 64-bit (unsigned long long) — counts fit for N ≤ 300.

  I/O:
  - Each input line has 1, 2, or 3 non-negative integers.
  - Blank lines are ignored. Process until EOF.
*/

const int MX = 300;

int main() {
	setIO();

  static unsigned long long dp[MX + 1][MX + 1] = {}; // dp[sum][#coins]
  dp[0][0] = 1ULL;

  for (int c = 1; c <= 300; ++c) {
    for (int s = c; s <= MX; ++s) {
      for (int k = 1; k <= MX; ++k) {
        dp[s][k] += dp[s - c][k - 1];
      }
    }
  }

  string line;
  while (true) {
    if (!getline(cin, line)) break;
    // Skip empty lines.
    bool only_spaces = true;
    for (char ch : line) if (!isspace((unsigned char)ch)) { only_spaces = false; break; }
    if (only_spaces) continue;

    stringstream ss(line);
    ll N; ss >> N;
    ll L1 = 0, L2 = MX; // defaults for "no limit" → 0..N
    if (ss.good()) {
      if (ss >> L1) {
        if (ss >> L2) {
          // 3 integers: N L1 L2
        } else {
          // 2 integers: N L1  => at most L1 coins
          L2 = L1;   // we'll clamp range [0..L1] below; easiest is to set L2=L1 and handle "≤ L1"
        }
      }
    }

    // Determine the range of k.
    unsigned long long ans = 0ULL;
    if (line.find_first_of(" \t") == string::npos) {
      // Only N provided: sum over all k = 0..N.
      int maxK = (int)min<ll>(N, MX);
      for (int k = 0; k <= maxK; ++k) ans += dp[N][k];
    } else {
      // Two or three integers.
      if (L2 == L1 && (int)count_if(all(line), [](char ch){ return isspace((unsigned char)ch); }) >= 1 && (int)count(all(line), ' ') + (int)count(all(line), '\t') >= 1) {
        // This branch is just to make intent explicit; logic below already handles both 2 or 3 ints correctly.
      }
      int kLo, kHi;
      // When two ints: interpret as ≤ L1  => [0..L1].
      // When three ints: interpret as [L1..L2].
      // We can detect 3 ints by checking if ss extracted L2 successfully above; but simpler:
      // If the original stream still had more ints, L2 is set; else L2==L1 but for "≤ L1" we want [0..L1].
      // We'll compute as:
      if (ss.fail()) {
        // Not reliable; instead parse tokens directly to know count.
      }

      // Re-parse to count tokens robustly.
      stringstream ss2(line);
      vector<ll> tok;
      ll t;
      while (ss2 >> t) tok.pb(t);

      if (sz(tok) == 1) {
        int maxK = (int)min<ll>(N, MX);
        for (int k = 0; k <= maxK; ++k) ans += dp[N][k];
      } else if (sz(tok) == 2) {
        ll lim = tok[1];
        int maxK = (int)min<ll>(min<ll>(lim, N), MX);
        for (int k = 0; k <= maxK; ++k) ans += dp[N][k];
      } else {
        ll a = tok[1], b = tok[2];
        if (a > b) swap(a, b);
        int lo = (int)max<ll>(0, a);
        int hi = (int)min<ll>(min<ll>(b, N), MX);
        for (int k = lo; k <= hi; ++k) ans += dp[N][k];
      }
    }

    cout << ans << '\n';
  }
}
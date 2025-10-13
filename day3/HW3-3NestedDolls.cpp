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

/*
  Goal:
  Given m dolls (w, h), a doll (w1, h1) fits in (w2, h2) iff w1 < w2 and h1 < h2.
  Partition all dolls into the minimum number of strictly increasing-by-(w,h) chains.

  Method (Dilworth / patience sorting):
  1) Sort by width ascending; for equal widths, sort height *descending*.
     - This prevents equal widths from nesting into each other.
  2) Now consider the sequence of heights.
     We need to cover this sequence with the minimum number of strictly increasing subsequences.
     By Dilworth's theorem, this number equals the length of the Longest Non-Increasing Subsequence (LNIS).
  3) Build piles greedily using a multiset of pile tops:
     For each height h in order:
       - Find the largest pile top that is < h (multiset.lower_bound(h) then step back).
       - If found, replace that pile's top with h (extend that increasing chain).
       - Otherwise, start a new pile with top h.
     The number of piles at the end is the answer.

  Complexity: O(m log m) per test case (m ≤ 20000).
*/

inline void solve() {
    int m; cin >> m;
    V<pi> a; a.reserve(m);
    for (int i = 0; i < m; ++i) {
      int w, h; cin >> w >> h;
      a.pb({w, h});
    }

    sort(all(a), [](const pi& A, const pi& B){
      if (A.fi != B.fi) return A.fi < B.fi;      // width ascending
      return A.se > B.se;                        // height descending on ties
    });

    multiset<int> piles; // each element is the top (last height) of a pile/chain
    for (auto x : a) {
        int w, h;
        tie(w, h) = x;
      auto it = piles.lower_bound(h); // first top >= h
      if (it == piles.begin()) {
        // no pile top < h -> start new pile
        piles.insert(h);
      } else {
        // extend the pile with the largest top < h
        --it;                  // now *it < h
        piles.erase(it);
        piles.insert(h);
      }
    }

    cout << sz(piles) << '\n';
}

int main() {
	setIO();
    int t = 1; cin >> t; // (comment out if one test case)
    while(t--) solve();
}
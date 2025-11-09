// Source: https://usaco.guide/general/io

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using db = long double;
template<class T> using V = vector<T>;
template<class T1, class T2> using P = pair<T1, T2>;
#define all(x) begin(x), end(x)
#define pb push_back
#define f first
#define s second
inline namespace FileIO {
	void setIn(string s){freopen(s.c_str(),"r",stdin);}
	void setOut(string s){freopen(s.c_str(),"w",stdout);}
	void setIO(string s = ""){
		cin.tie(0)->sync_with_stdio(0);
		cin.exceptions(cin.failbit);
		if (size(s)) setIn(s+".in"), setOut(s+".out");
	}
}
const db PI = acos((db)-1);

const int NMAX = 250;
const int WMAX = 1000;
const ll INF = 1e18;

int weights[NMAX];
int talent[NMAX];

int n;
int w;

// The dp state.
// For 0 <= i < w, this is the maximum adjusted-talent-score achievable
// with weight exactly i.
// For i=w, this is the maximum talent achievable
// with weight AT LEAST w.
ll dp[WMAX + 1];

// Check if a ratio of y/1000 is achievable.
bool doable(int y) {
  for(int i = 0; i <= w; i++) dp[i] = -INF;
  dp[0] = 0;

  for (int j = 0; j < n; j++) {
    ll value = 1000*(ll)talent[j] - y*(ll)weights[j];
    int inc = weights[j];
    for (int k = w; k >= 0; k--) {
      int k1 = min(w, k + inc);
      if (dp[k] != -INF) {
        if (dp[k1] < dp[k] + value) {
          dp[k1] = dp[k] + value;
        }
      }
    }
  }

  return dp[w] >= 0;
}

int main() {
  setIO("talent");
  cin >> n >> w;
  assert(1 <= n && n <= NMAX);
  assert(1 <= w && w <= WMAX);

  for (int i = 0; i < n; i++) {
    cin >> weights[i] >> talent[i];
    assert(1 <= weights[i] && weights[i] <= 99951);
    assert(1 <= talent[i] && talent[i] <= 1000);
  }

  // Binary search
  // Invariant: lo <= answer < hi
  int lo = 0;
  int hi = (1000 * 250 * 1000) + 1;
  while (hi > lo + 1) {
    int mid = (lo + hi) / 2;
    if (doable(mid)) {
      lo = mid;
    } else {
      hi = mid;
    }
  }
  cout << lo;
}
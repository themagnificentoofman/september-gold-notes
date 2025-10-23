// Source: https://usaco.guide/general/io
// #pragma GCC optimize("Ofast")
// #pragma GCC target("avx2")

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using db = long double; // change to double if TL is tight (almost never)
template<class T> using V = vector<T>;
template<class T1, class T2> using P = pair<T1, T2>;
#define bg(x) begin(x)
#define all(x) bg(x), end(x)
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) (int)(x).size()
#define rsz resize
#define ft front()
#define bk back()
#define pb push_back
#define pf push_front
#define ppb pop_back
#define ppf pop_front
#define pp pop
#define ins insert
#define fi first
#define se second
using vi = V<int>;
using vl = V<ll>;
using pi = P<int,int>;
using pl = P<ll,ll>;

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
const int MOD = (int)1e9+7; // 998244353;
const int MX = (int)2e5+5; // change as needed

/*
 (1) Problem recap (simple):
     Given a line of integers per test case, can we split them into two groups with equal sum?

 (2) Things to notice & step-by-step:
     - If total sum is odd → impossible immediately.
     - If total sum is even, target = total/2.
     - Classic subset-sum: dp[s] = can we reach sum s using some numbers?
     - 1D DP update backward to avoid reusing the same item multiple times.

 (3) Solution outline:
     - Parse the line to get all numbers and compute total.
     - If total%2==1 → print "NO".
     - Else run 1D DP size target+1 with dp[0]=true, update backwards for each x.
     - Answer is dp[target].
*/

inline void solve() {
	string s;
	getline(cin, s);
	stringstream ss(s);
	vi a; a.reserve(21);
	ll tot = 0;
	for (int x; ss >> x; ) {
		a.pb(x);
		tot += x;
	}
	if (tot & 1) {
		cout << "NO\n";
		return;
	}
	int target = tot / 2;
	// dp[s] == 1 if we can make sum s.
	V<bool> dp(target + 1, 0);
	dp[0] = 1;
	for (int x : a) {
		// If negatives can appear, this needs a different approach, but weights are always nonnegative.
		// if (x > target) x = x; // no-op (loop skips automatically)
		for (int s = target; s >= x; --s) {
			// dp[s] = dp[s] OR dp[s-x].
			if (dp[s - x]) dp[s] = 1;
		}
	}
	cout << (dp[target] ? "YES" : "NO") << '\n';
}

int main() {
    setIO();
    int t = 1; cin >> t; // (comment out if not needed)
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // drop the rest of the line after t
    while(t--) solve();
}
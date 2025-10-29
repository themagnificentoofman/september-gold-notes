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
		// cin.exceptions(cin.failbit); // comment out if breaks things
		if (sz(s)) setIn(s+".in"), setOut(s+".out");
	}
}
const db PI = acos((db)-1);
const int MOD = (int)1e9+7; // 998244353;
const int MX = (int)2e5+5; // change as needed

int lcs(string a, string b) {
    vi dp(sz(b)+1);
	for (char c : a) {
	    for (int i = sz(b)-1; i >= 0; i--) if (b[i] == c) dp[i+1] = dp[i]+1;
	    for (int i = 0; i < sz(b); i++) dp[i+1] = max(dp[i+1], dp[i]);
	}
	return dp[sz(b)];
}

int main() {
    setIO();
    string s1, s2;
    while(getline(cin, s1) && getline(cin, s2)) {
        // remove trailing '\r' if present (Windows line endings)
        if (!s1.empty() && s1.back() == '\r') s1.pop_back();
        if (!s2.empty() && s2.back() == '\r') s2.pop_back();
        cout << lcs(s1, s2) << '\n';
    }
}
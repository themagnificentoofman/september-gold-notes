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

inline void solve() {
    string s; cin >> s;
    auto min_partitions = [&](const string& str) -> int {
        /*
         We want the minimum number of palindromic substrings whose concatenation is str.
         Let dp[i] = minimum partitions to cover prefix str[0..i-1].
         dp[0] = 0 (empty string needs 0 pieces).
         Whenever str[l..r] is a palindrome, we can do:
             dp[r+1] = min(dp[r+1], dp[l] + 1).

         We'll generate all palindromes by expanding around every center (odd + even).
         This is O(n^2) time, O(n) memory.
        */
        int n = sz(str);
        vi dp(n+1, INT32_MAX);
        dp[0] = 0;
        for (int c = 0; c < n; c++) {
            // odd-length palindromes centered at c
            int l = c, r = c;
            while (l >= 0 && r < n && str[l] == str[r]) {
                dp[r+1] = min(dp[r+1], dp[l] + 1);
                l--; r++;
            }
            // even-length palindromes centered between c and c+1
            l = c; r = c+1;
            while (l >= 0 && r < n && str[l] == str[r]) {
                dp[r+1] = min(dp[r+1], dp[l] + 1);
                l--; r++;
            }
        }
        return dp[n];
    };
    cout << min_partitions(s) << '\n';
}

int main() {
    setIO();
    int t = 1; cin >> t;
    while(t--) solve();
}
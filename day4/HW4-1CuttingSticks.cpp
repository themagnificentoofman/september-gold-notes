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

int memo[51][51]; // Memoization
vi v; // Stores cutting points

int min_cost(int a, int b) { // Takes two indices of cutting points (indices of v)
    if (b - a == 1) return 0; // Base case (b next to a): Already cut down to what the segment should look like at the end; no need to cut anymore.
    if (memo[a][b] != -1) return memo[a][b];
    int ret = INT32_MAX;
    for (int i = a + 1; i < b; i++) // For all possible splitting points i between a and b:
        ret = min(ret, v[b] - v[a] + min_cost(a, i) + min_cost(i, b)); // Minimize (cost of splitting this segment + min_cost(a, i) + min_cost(i, b))
    memo[a][b] = ret;
    return ret;
}

int main() {
    setIO();
    int len, n;
    for (;;) {
        cin >> len;
        if (!len) break;
        cin >> n;
        v.clear();
        v.pb(0);
        for (int i = 0; i < n; i++) {
            int c; cin >> c;
            v.pb(c);
        }
        v.pb(len);
        memset(memo, -1, sizeof(memo));
        cout << "The minimum cutting is " << min_cost(0, n+1) << ".\n";
    }
}
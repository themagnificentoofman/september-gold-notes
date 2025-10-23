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
 Traveling Salesman Problem with Manhattan Distance as Cost
 - Time Complexity: O(2^n * n^2)
 - n cities with 2^n bitmask visited states
 - Each state has a for loop than takes O(n)
*/

int n, xsz, ysz, x[11], y[11];
int man[11][11], memo[11][1<<11];

int salesman(int pos, int mask) {
    // Base case: last to first city
    if (mask == ((1<<(n+1))-1)) return man[pos][0];
    if (memo[pos][mask] != -1) return memo[pos][mask];
    int res = INT32_MAX;
    for (int nxt = 1; nxt <= n; nxt++) // try all next locations
        if (!(mask&(1<<nxt))) // if not in visited mask already
            res = min(res, salesman(nxt, mask|(1<<nxt)) + man[pos][nxt]);
    return memo[pos][mask] = res;
}

inline void solve() {
    cin >> xsz >> ysz >> x[0] >> y[0] >> n;
    for (int i = 1; i <= n; i++) {
        cin >> x[i] >> y[i];
    }
    // compute man dist between cities
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            man[i][j] = abs(x[i]-x[j]) + abs(y[i]-y[j]);
    memset(memo, -1, sizeof(memo));
    cout << "The shortest path has length " << salesman(0, 1) << '\n';
}

int main() {
    setIO();
    int t = 1; cin >> t; // (comment out if not needed)
    while(t--) solve();
}
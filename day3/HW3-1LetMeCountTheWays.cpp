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

int coins[]{1,5,10,25,50};
const int MX = 3e4;

int main() {
	setIO();
    V<unsigned long long> dp(MX + 1, 0);
    dp[0] = 1;

    for (int c : coins) {
        for (int s = c; s <= MX; ++s) {
            dp[s] += dp[s - c];
        }
    }

    int n;
    while (cin >> n) {
        unsigned long long m = dp[n];
    if (m == 1)
        cout << "There is only 1 way to produce " << n << " cents change.\n";
    else
        cout << "There are " << m << " ways to produce " << n << " cents change.\n";
    }
}
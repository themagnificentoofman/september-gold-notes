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

/*
 (1) Problem recap in simple terms:
     We're given n matrices A1..An. Ai has size r[i] x c[i].
     We want to fully parenthesize A1 x A2 x ... x An so that the
     total number of scalar multiplications is minimal.
     We must print the optimal parenthesization like "(A1 x (A2 x A3))".

 (2) Things to notice and step-by-step derivation:
     - Multiplying a (p x q) matrix with a (q x r) matrix costs p*q*r multiplications.
     - Matrix multiplication is associative, so we can choose where to put parentheses.
     - Let dp[i][j] = minimal cost to compute Ai..Aj as one matrix.
       Base case: dp[i][i] = 0 because a single matrix needs no multiplication.
     - For a chain Ai..Aj (i < j), we try every split k between i..j-1:
           cost = dp[i][k] + dp[k+1][j] + r[i] * c[k] * c[j]
       We take the minimum. We also remember which k was best so we can reconstruct
       the parenthesization.

 (3) Solution outline:
     - Read input cases until n == 0.
     - Build dp and split tables (size n x n).
     - Bottom-up over increasing segment length.
     - Reconstruct the optimal parenthesization with a recursive function that uses
       the 'split' table. Print "Case X: <parenthesization>".
*/

int n, caseNum = 1;

string buildParen(int i, int j, const V<vi>& split) {
	if (i == j) {
		return "A" + to_string(i);
	}
	int k = split[i][j];
	return "(" + buildParen(i, k, split) + " x " + buildParen(k+1, j, split) + ")";
}

inline void solve() {
    vi r(n+1), c(n+1);
    for (int i = 1; i <= n; i++)
        cin >> r[i] >> c[i];
    /* dp[i][j] = min cost to multiply Ai..Aj
        split[i][j] = k where we split (Ai..Ak) x (A(k+1)..Aj)
    */
    V<vl> dp(n+1, vl(n+1, 0));
    V<vi> split(n+1, vi(n+1, -1));
    for (int len = 2; len <= n; len++) {
        for (int i = 1; i + len - 1 <= n; i++) {
            int j = i + len - 1;
            dp[i][j] = INT64_MAX; // inf
            for (int k = i; k < j; k++) {
                /*
                    cost of:
                    (Ai..Ak) then (A(k+1)..Aj) then multiply the two results.
                    Dimensions:
                    - Ai..Ak is r[i] x c[k]
                    - A(k+1)..Aj is r[k+1] x c[j] = c[k] x c[j]  (must match)
                    Multiply cost: r[i] * c[k] * c[j]
                */
                ll cost = dp[i][k] + dp[k+1][j] + (ll)r[i] * (ll)c[k] * (ll)c[j];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    split[i][j] = k;
                }
            }
        }
    }
    string ans = buildParen(1, n, split);
    cout << "Case " << caseNum++ << ": " << ans << "\n";
}

int main() {
	setIO();
	for (;;) {
		cin >> n;
		if (n == 0) break;
        solve();
	}
}
// Source: https://usaco.guide/general/io
// #pragma GCC optimize("Ofast")
// #pragma GCC target("avx2")

#include <bits/stdc++.h>
using namespace std;

// basic type aliases
using str = string;
using ll = long long;
using i128 = __int128_t;
using db = long double;

// pairs
template<class T1, class T2> using P = pair<T1,T2>;
#define mp make_pair
#define fi first
#define se second
using pi = P<int,int>;
using pl = P<ll,ll>;
using pd = P<db,db>;

// vectors
template<class T> using V = vector<T>;
#define sz(x) int((x).size())
#define bg(x) begin(x)
#define all(x) bg(x), end(x)
#define rall(x) (x).rbegin(), (x).rend()
#define sor(x) sort(all(x))
#define rsz resize
#define ins insert
#define pb push_back
#define eb emplace_back
#define ft front()
#define bk back()
using vi = V<int>;
using vb = V<bool>;
using vl = V<ll>;
using vd = V<db>;
using vs = V<str>;
using vpi = V<pi>;
using vpl = V<pl>;
using vpd = V<pd>;

// misc. ops & aliases
#define pf push_front
#define ppb pop_back
#define ppf pop_front
#define lb lower_bound
#define ub upper_bound
template<class T, size_t SZ> using AR = array<T,SZ>;
template<class T> int lwb(V<T>& a, const T& b) { return int(lb(all(a),b)-bg(a)); }
template<class T> int upb(V<T>& a, const T& b) { return int(ub(all(a),b)-bg(a)); }
template<class T> using pq = priority_queue<T>;
template<class T> using pqg = priority_queue<T,V<T>,greater<T>>;

// input & output
inline namespace FileIO {
    void setIn(string s){freopen(s.c_str(),"r",stdin);}
    void setOut(string s){freopen(s.c_str(),"w",stdout);}
    void setIO(string s = ""){
        cin.tie(0)->sync_with_stdio(0);
        cin.exceptions(cin.failbit);
        if (sz(s)) setIn(s+".in"), setOut(s+".out");
    }
}

// constants
const db PI = acos((db)-1);
const ll BIG = (ll)4e18; // large INF for ll
const int MAXN = 4005;

// prefix sums of u for cost on intervals
static ll pref[MAXN][MAXN];

// dp arrays for current and previous group counts
static ll dp_arr[2][MAXN];
static ll *dp_prev, *dp_cur;

int n, k;

/*
 cost(l, r):
 - Returns the unfamiliarity cost of putting people [l..r] into one gondola.
 - u_ij is symmetric and u_ii = 0.
 - pref[i][j] = sum_{x=1..i} sum_{y=1..j} u[x][y].
 - Sum over the square [l..r] x [l..r] is:
       S = pref[r][r] - pref[l-1][r] - pref[r][l-1] + pref[l-1][l-1].
   This counts each unordered pair {i,j} with i != j twice (once as (i,j) and once as (j,i)),
   and diagonal (i,i) is zero. So cost(l,r) = S / 2.
*/
inline ll cost(int l, int r) {
    if (l >= r) return 0; // 0 or 1 person -> no pairs.
    ll S = pref[r][r]
         - pref[l-1][r]
         - pref[r][l-1]
         + pref[l-1][l-1];
    return S / 2;
}

/*
 compute(g, L, R, optL, optR):
 - We are computing dp[g][i] for i in [L..R], storing into dp_cur[i].
 - Transition:
       dp[g][i] = min_{p in [g-1..i-1]} ( dp[g-1][p] + cost(p+1, i) ).
 - Monotone (Knuth/Monge) property:
       The optimal p for dp[g][i] is non-decreasing in i.
 - Therefore, for i in [L..R], if we know the optimal p for some mid,
   we can restrict the search range for the left and right halves.
*/
void compute(int g, int L, int R, int optL, int optR) {
    if (L > R) return;
    int mid = (L + R) >> 1;

    ll bestVal = BIG;
    int bestPos = -1;

    int start = max(optL, g - 1);     // p must be >= g-1
    int finish = min(optR, mid - 1);  // and < mid

    for (int p = start; p <= finish; ++p) {
        ll cand = dp_prev[p] + cost(p + 1, mid);
        if (cand < bestVal) {
            bestVal = cand;
            bestPos = p;
        }
    }

    dp_cur[mid] = bestVal;

    compute(g, L, mid - 1, optL, bestPos);
    compute(g, mid + 1, R, bestPos, optR);
}

inline void solve() {
    cin >> n >> k;

    // Build 2D prefix sums of u on the fly.
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            char x; cin >> x; // inputting as int TLEs for some reason :/
            pref[i][j] = pref[i-1][j] + pref[i][j-1] - pref[i-1][j-1] + (x - '0');
        }
    }

    // Base case DP for g = 1: exactly one gondola for first i people -> [1..i].
    int row1 = 1;
    dp_prev = dp_arr[row1];
    dp_prev[0] = 0;
    for (int i = 1; i <= n; ++i) {
        dp_prev[i] = cost(1, i);
    }

    // For g from 2 to k, use divide-and-conquer on monotone argmin.
    for (int g = 2; g <= k; ++g) {
        int curRow = g & 1;
        int prevRow = (g - 1) & 1;
        dp_prev = dp_arr[prevRow];
        dp_cur  = dp_arr[curRow];

        // Compute dp[g][i] for i in [g..n]. For i < g, the state is invalid (at least g people needed).
        compute(g, g, n, g - 1, n - 1);
    }

    // If k == 1, the answer is dp_arr[1][n]. If k > 1, it is on row (k & 1).
    cout << dp_arr[k & 1][n];
}

int main() {
    setIO();
    solve(); // single case
}
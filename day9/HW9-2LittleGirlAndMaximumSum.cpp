// Source: https://usaco.guide/general/io
// #pragma GCC optimize("Ofast")
// #pragma GCC target("avx2")

#include <bits/stdc++.h>
using namespace std;

// basic type aliases
using str = string;
using ll = long long;
using i128 = __int128_t; // sometimes will use i128 if really needed
using db = long double;  // change to double if TL is tight (almost never)

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
using vi  = V<int>;
using vb  = V<bool>;
using vl  = V<ll>;
using vd  = V<db>;
using vs  = V<str>;
using vpi = V<pi>;
using vpl = V<pl>;
using vpd = V<pd>;

// misc. ops & aliases
#define pf  push_front
#define ppb pop_back
#define ppf pop_front
#define lb  lower_bound
#define ub  upper_bound
template<class T, size_t SZ> using AR = array<T,SZ>;
template<class T> int lwb(V<T>& a, const T& b) { return int(lb(all(a),b) - bg(a)); }
template<class T> int upb(V<T>& a, const T& b) { return int(ub(all(a),b) - bg(a)); }
template<class T> using pq  = priority_queue<T>;
template<class T> using pqg = priority_queue<T,V<T>,greater<T>>;

// input & output
inline namespace FileIO {
	void setIn(string s){ freopen(s.c_str(),"r",stdin); }
	void setOut(string s){ freopen(s.c_str(),"w",stdout); }
	void setIO(string s = ""){
		cin.tie(nullptr)->sync_with_stdio(false);
		cin.exceptions(cin.failbit); // comment out if it breaks things
		if (sz(s)) setIn(s + ".in"), setOut(s + ".out");
	}
}

// constants (add more if needed)
const db PI  = acos((db)-1);
const ll BIG = 4e18;
const int MOD = (int)1e9+7;
const int MXN = (int)2e5+5;

/*
 (1) Problem recap in simple terms:
     We have an array a[1..n] and q range-sum queries [l, r].
     We are allowed to reorder the array once, before answering all queries,
     to maximize the total sum of all query answers.

 (2) Things to notice and step-by-step derivation:
     - Each position i in the array is included in some number cnt[i] of queries.
       Its value contributes a[i] * cnt[i] to the final total.
     - Reordering lets us assign the largest numbers to positions with largest cnt[i].
     - So the problem reduces to:
         * Compute how many queries cover each index (cnt[i]).
         * Sort cnt in descending order.
         * Sort the array values in descending order.
         * Match them pairwise and sum a_sorted[i] * cnt_sorted[i].
     - To compute cnt efficiently:
         * Use a difference array diff[ ] over indices 0..n.
         * For each query [l, r] (1-based):
               diff[l-1]++, diff[r]--.
           Then prefix-sum diff to get cnt[0..n-1].

 (3) Solution outline:
     - Read n, q and the array a (store 0-based).
     - Build diff[0..n] initialized to 0.
     - For each query [l, r]:
           diff[l-1]++, diff[r]--.
     - Build cnt[i] as prefix sum of diff.
     - Sort a in descending order, sort cnt in descending order.
     - Compute ans = Σ a[i] * cnt[i] over i = 0..n-1 and print it.
*/

inline void solve() {
	int n, q;
    cin >> n >> q;

	vl a(n);
	for (int i = 0; i < n; i++) cin >> a[i];

	vl diff(n + 1, 0);
	for (int qi = 0; qi < q; qi++) {
		int l, r;
		cin >> l >> r;          // 1-based
		diff[l - 1] += 1;       // start coverage at l-1 (0-based index)
		diff[r]     -= 1;       // end coverage after r-1
	}

	vl cnt(n, 0);
	ll cur = 0;
	for (int i = 0; i < n; i++) {
		cur += diff[i];
		cnt[i] = cur;
	}

	sort(rall(a));
	sort(rall(cnt));

	ll ans = 0;
	for (int i = 0; i < n; i++) {
		ans += a[i] * cnt[i];
	}

	cout << ans << '\n';
}

int main() {
	setIO();
	solve(); // single case
}
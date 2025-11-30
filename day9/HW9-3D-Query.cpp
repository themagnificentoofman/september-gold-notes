// Source: https://usaco.guide/general/io
// #pragma GCC optimize("Ofast")
// #pragma GCC target("avx2")

#include <bits/stdc++.h>
using namespace std;

// basic type aliases
using str = string;
using ll = long long;
using i128 = __int128_t; // use i128 when really needed
using db = long double;  // change to double if TL is tight

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
const int MXN = (int)3e4 + 5;

/*
 (1) Problem recap in simple terms:
     We have an array a[1..n] and q queries. Each query gives (i, j) and asks:
     “How many distinct values appear in the subarray a[i..j]?”

 (2) Things to notice and step-by-step derivation:
     - Constraints: n ≤ 30000, q ≤ 200000, so we can’t afford O(nq).
     - Classic offline trick:
         * Process positions from left to right.
         * For each value, we keep only its latest occurrence active.
     - Use a Fenwick tree (BIT) over indices:
         * When we see a value at position pos:
             - If this value appeared before at old_pos, we remove its contribution at old_pos.
             - Add contribution at pos.
         * At any time, BIT[i] = 1 if index i currently holds the latest occurrence of its value, else 0.
         * For a query (l, r), the number of distinct values in a[l..r] equals
             sumBIT(r) - sumBIT(l-1).
     - To answer many queries efficiently:
         * Sort queries by their right endpoint r in increasing order.
         * Sweep pos from 1 to n and update BIT.
         * When we reach r, all contributions for prefix [1..r] are ready; answer all queries with that r.

 (3) Solution outline:
     - Read n and the array.
     - Read q queries, store as (l, r, id).
     - Sort queries by r.
     - Maintain:
         * last[value] = last index where this value appeared (0 if never).
         * Fenwick tree over [1..n].
     - Sweep pos = 1..n:
         * Let x = a[pos]. If last[x] != 0, BIT.add(last[x], -1).
         * BIT.add(pos, +1), set last[x] = pos.
         * While there are queries with r == pos, answer them using BIT range sum.
     - Output answers in original query order.
*/

// Fenwick tree for point updates and prefix sums on [1..n].
struct Fenwick {
	int n;
	vi bit;

	Fenwick(int _n = 0) { init(_n); }

	void init(int _n) {
		n = _n;
		bit.assign(n + 1, 0);
	}

	void add(int idx, int val) {
		for (; idx <= n; idx += idx & -idx) bit[idx] += val;
	}

	int sumPrefix(int idx) const {
		int res = 0;
		for (; idx > 0; idx -= idx & -idx) res += bit[idx];
		return res;
	}

	int rangeSum(int l, int r) const {
		if (l > r) return 0;
		return sumPrefix(r) - sumPrefix(l - 1);
	}
};

struct Query {
	int l, r, id;
};

inline void solve() {
	int n;
	cin >> n;

	vi a(n + 1);
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}

	int q;
	cin >> q;
	V<Query> qs(q);
	for (int i = 0; i < q; i++) {
		int l, r;
		cin >> l >> r;
		qs[i] = {l, r, i};
	}

	// sort queries by increasing r
	sort(all(qs), [](const Query &A, const Query &B) {
		return A.r < B.r;
	});

	Fenwick fw(n);
	// ai ≤ 1e6 => we can use a direct array for last occurrence
	static int last[1000001];
	for (int i = 0; i <= 1000000; i++) last[i] = 0;

	vi ans(q, 0);
	int qi = 0;

	for (int pos = 1; pos <= n; pos++) {
		int x = a[pos];
		if (last[x] != 0) {
			fw.add(last[x], -1); // old position no longer latest
		}
		fw.add(pos, +1);          // new latest position
		last[x] = pos;

		// answer all queries with right endpoint = pos
		while (qi < q && qs[qi].r == pos) {
			const auto &qq = qs[qi];
			ans[qq.id] = fw.rangeSum(qq.l, qq.r);
			qi++;
		}
	}

	for (int i = 0; i < q; i++) {
		cout << ans[i] << '\n';
	}
}

int main() {
	setIO();
	solve(); // single case
}

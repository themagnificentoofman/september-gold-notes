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
template<class T> int lwb(V<T>& a, const T& b) { return int(lb(all(a),b) - bg(a)); }
template<class T> int upb(V<T>& a, const T& b) { return int(ub(all(a),b) - bg(a)); }
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
const ll BIG = 4e18;
const int MOD = (int)1e9+7;
const int MXN = (int)2e5+5;

/*
 (1) Problem recap in simple terms:
     We have an array a[1..n] and q queries. Each query gives (i, j, k) and asks:
     “How many elements in a[i..j] are strictly greater than k?”

 (2) Things to notice and step-by-step derivation:
     - Constraints: n ≤ 30000, q ≤ 200000. A naive O(nq) approach is too slow.
     - We only compare array values to k, not between queries, so we can work offline:
         * Sort array positions by a[pos] in descending order.
         * Sort queries by k in descending order.
     - We maintain a Fenwick tree (BIT) over indices 1..n:
         * When we move through the array (from largest values to smaller),
           we "activate" positions whose value > current k by adding 1 in the BIT.
         * For a query with threshold k, if we have already inserted all a[pos] > k,
           then the BIT stores 1 exactly at those positions.
         * Answer for (i, j, k) = sumBIT(j) - sumBIT(i-1).

 (3) Solution outline:
     - Read n and the array.
     - Build a list of (value, index) and sort it in decreasing value.
     - Read all q queries, store them as (i, j, k, id), and sort by k in decreasing order.
     - Maintain a Fenwick tree of size n and a pointer ptr over the sorted array values.
     - For each query in sorted order:
         * While ptr < n and value_list[ptr].value > query.k:
               - Add 1 to BIT at position value_list[ptr].index and move ptr.
         * Query BIT range [i, j] and store the result as answer[id].
     - Finally, output answers in original order.
*/

// Fenwick tree for prefix sums on [1..n].
struct Fenwick {
	int n;
	vl bit;
	Fenwick(int _n = 0) { init(_n); }

	void init(int _n) {
		n = _n;
		bit.assign(n + 1, 0);
	}

	void add(int idx, ll val) {
		for (; idx <= n; idx += idx & -idx) bit[idx] += val;
	}

	ll sumPrefix(int idx) const {
		ll res = 0;
		for (; idx > 0; idx -= idx & -idx) res += bit[idx];
		return res;
	}

	ll rangeSum(int l, int r) const {
		if (l > r) return 0;
		return sumPrefix(r) - sumPrefix(l - 1);
	}
};

struct Query {
	int l, r;
	ll k;
	int id;
};

inline void solve() {
	int n;
    cin >> n;

	V<P<ll,int>> vals;
	vals.rsz(n);
	for (int i = 0; i < n; i++) {
		ll x;
		cin >> x;
		vals[i] = {x, i + 1}; // store 1-based index
	}

	int q;
	cin >> q;
	V<Query> queries(q);
	for (int qi = 0; qi < q; qi++) {
		int l, r;
		ll k;
		cin >> l >> r >> k;
		queries[qi] = {l, r, k, qi};
	}

	sort(rall(vals)); // now vals is in descending order by value

	// Sort queries descending by k.
	sort(all(queries), [](const Query& A, const Query& B) {
		return A.k > B.k;
	});

	Fenwick fw(n);
	vl ans(q, 0);

	int ptr = 0; // pointer over vals (descending)

	for (const auto &qq : queries) {
		// Insert into Fenwick all positions with a[pos] > qq.k
		while (ptr < n && vals[ptr].fi > qq.k) {
			int pos = vals[ptr].se;
			fw.add(pos, 1); // mark this index as "value > current k"
			ptr++;
		}
		ans[qq.id] = fw.rangeSum(qq.l, qq.r);
	}

	for (int i = 0; i < q; i++) {
		cout << ans[i] << '\n';
	}
}

int main() {
	setIO();
	solve(); // single case
}
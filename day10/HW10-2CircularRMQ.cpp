// Source: https://usaco.guide/general/io
// #pragma GCC optimize("Ofast")
// #pragma GCC target("avx2")

#include <bits/stdc++.h>
using namespace std;

// basic type aliases
using str = string;
using ll = long long;
using i128 = __int128_t; // sometimes will use lll, but can easily get mixed up w/ ll (use i128 by default)
using db = long double; // change to double if TL is tight (almost never)

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
// lower_bound / upper_bound convenience; use on sorted containers
template<class T, size_t SZ> using AR = array<T,SZ>; // c-style array but better, basically (can use .at())
template<class T> int lwb(V<T>& a, const T& b) { return int(lb(all(a),b)-bg(a)); } // ind of 1st elem >= b
template<class T> int upb(V<T>& a, const T& b) { return int(ub(all(a),b)-bg(a)); } // ind of 1st elem > b
template<class T> using pq = priority_queue<T>; // max-heap priority queue (largest on top)
template<class T> using pqg = priority_queue<T,V<T>,greater<T>>; // min-heap priority queue (smallest on top)

// input & output
inline namespace FileIO {
	void setIn(string s){freopen(s.c_str(),"r",stdin);}
	void setOut(string s){freopen(s.c_str(),"w",stdout);}
	void setIO(string s = ""){
		cin.tie(0)->sync_with_stdio(0);
		// cin.exceptions(cin.failbit); // comment out if breaks things
		if (sz(s)) setIn(s+".in"), setOut(s+".out");
	}
}

// constants (add more if needed)
const db PI = acos((db)-1);
const ll BIG = 4e18; // large enough for ll, not close to INT64_MAX tho
const int MOD = (int)1e9+7; // 998244353;
const int MXN = (int)2e5+5; // modify as needed

// customizable lazy segment tree: range add, range min query
struct SegmentTree {
	int n;
	vl seg, lazy;

	ll cmb(ll a, ll b) {
		return min(a, b);
	}

	void init(int _n) {
		n = 1;
		while (n < _n) n <<= 1;
		seg.assign(2 * n, BIG);
		lazy.assign(2 * n, 0);
	}

	void build(const vl& a) {
		int _n = sz(a);
		init(_n);
		for (int i = 0; i < _n; ++i) seg[n + i] = a[i];
		for (int p = n - 1; p >= 1; --p) pull(p);
	}

	void pull(int p) {
		seg[p] = cmb(seg[p << 1], seg[p << 1 | 1]);
	}

	void apply(int p, ll v) {
		seg[p] += v;
		lazy[p] += v;
	}

	void push(int p) {
		if (lazy[p] != 0) {
			apply(p << 1, lazy[p]);
			apply(p << 1 | 1, lazy[p]);
			lazy[p] = 0;
		}
	}

	void rangeAdd(int ql, int qr, ll v, int p, int l, int r) {
		if (qr < l || r < ql) return;
		if (ql <= l && r <= qr) {
			apply(p, v);
			return;
		}
		push(p);
		int m = (l + r) >> 1;
		rangeAdd(ql, qr, v, p << 1, l, m);
		rangeAdd(ql, qr, v, p << 1 | 1, m + 1, r);
		pull(p);
	}

	ll rangeMin(int ql, int qr, int p, int l, int r) {
		if (qr < l || r < ql) return BIG;
		if (ql <= l && r <= qr) return seg[p];
		push(p);
		int m = (l + r) >> 1;
		return cmb(rangeMin(ql, qr, p << 1, l, m),
		           rangeMin(ql, qr, p << 1 | 1, m + 1, r));
	}

	// public wrappers
	void rangeAdd(int l, int r, ll v) {
		rangeAdd(l, r, v, 1, 0, n - 1);
	}

	ll query(int l, int r) {
		return rangeMin(l, r, 1, 0, n - 1);
	}
};

inline void solve() {
	int n;
	if (!(cin >> n)) return;
	vl a(n);
	for (int i = 0; i < n; ++i) cin >> a[i];

	SegmentTree st;
	st.build(a);

	int m;
	cin >> m;
	string line;
	getline(cin, line); // consume endline after m

	for (int i = 0; i < m; ++i) {
		getline(cin, line);
		if (line.empty()) { --i; continue; }

		stringstream ss(line);
		vector<ll> tok;
		ll x;
		while (ss >> x) tok.pb(x);

		if (sz(tok) == 2) {
			// rmq(lf, rg)
			int lf = (int)tok[0];
			int rg = (int)tok[1];
			if (lf <= rg) {
				cout << st.query(lf, rg) << '\n';
			} else {
				ll ans = min(st.query(lf, n - 1), st.query(0, rg));
				cout << ans << '\n';
			}
		} else {
			// inc(lf, rg, v)
			int lf = (int)tok[0];
			int rg = (int)tok[1];
			ll v = tok[2];
			if (lf <= rg) {
				st.rangeAdd(lf, rg, v);
			} else {
				st.rangeAdd(lf, n - 1, v);
				st.rangeAdd(0, rg, v);
			}
		}
	}
}

int main() {
    setIO();
    solve(); // single case
}
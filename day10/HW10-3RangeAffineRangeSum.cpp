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
		cin.exceptions(cin.failbit); // comment out if breaks things
		if (sz(s)) setIn(s+".in"), setOut(s+".out");
	}
}

// constants (add more if needed)
const db PI = acos((db)-1);
const ll BIG = 4e18; // large enough for ll, not close to InT64_MAX tho
const int MOD = 998244353;
const int MXN = (int)2e5+5; // modify as needed

/*
 1) We have an array and need to support up to 5e5 operations of two types: range affine updates
    a[i] = b * a[i] + c and range sum queries, all modulo 998244353.
 2) The key observation is that both operations are linear. For a segment [l, r), if we apply
    x -> m * x + p to every element, the segment sum transforms as:
    S' = m * S + p * (r - l). Also, composing two affine maps is still affine, so we can store
    lazy tags as (mul, add) and update them with simple formulas.
 3) Build a lazy segment tree that stores the sum for each node plus a pending affine tag.
    Each range update composes the new (b, c) with the node's existing tag and updates its sum
    in O(1). queries push tags down as needed and sum over at most O(log n) nodes.
    Overall complexity is O((n + q) log n).
*/

struct SegTree {
	int n;
	vl seg; // segment sums
	vl mul; // lazy mul tag
	vl add; // lazy add tag

	SegTree(int _n) {
		n = _n;
		seg.assign(4 * n, 0LL);
		mul.assign(4 * n, 1LL); // identity: x -> 1 * x + 0
		add.assign(4 * n, 0LL);
	}

	void build(int idx, int l, int r, const vl &a) {
		if (r - l == 1) {
			if (l < n) seg[idx] = a[l] % MOD;
			return;
		}
		int m = l + r >> 1;
		build(idx << 1, l, m, a);
		build(idx << 1 | 1, m, r, a);
		pull(idx);
	}

	// apply affine transform x -> b * x + c to node covering [l, r)
	void apply_node(int idx, int l, int r, ll b, ll c) {
		ll len = (ll)(r - l);
		seg[idx] = ((b * seg[idx]) % MOD + (c % MOD) * (len % MOD)) % MOD;
		mul[idx] = (b * mul[idx]) % MOD;
		add[idx] = ((b * add[idx]) % MOD + c) % MOD;
	}

	// push lazy tag from this node to children
	void push(int idx, int l, int r) {
		if (mul[idx] == 1 && add[idx] == 0) return; // nothing to push
		if (r - l <= 1) return; // leaf
		int m = l + r >> 1;
		apply_node(idx << 1, l, m, mul[idx], add[idx]);
		apply_node(idx << 1 | 1, m, r, mul[idx], add[idx]);
		mul[idx] = 1LL;
		add[idx] = 0LL;
	}

    // pull updates from children
    void pull(int idx) {
        seg[idx] = seg[idx << 1] + seg[idx << 1 | 1];
		if (seg[idx] >= MOD) seg[idx] -= MOD;
    }

	// range apply affine transform on [ql, qr)
	void range_apply(int idx, int l, int r, int ql, int qr, ll b, ll c) {
		if (qr <= l || r <= ql) return;
		if (ql <= l && r <= qr) {
			apply_node(idx, l, r, b, c);
			return;
		}
		push(idx, l, r);
		int m = l + r >> 1;
		range_apply(idx << 1, l, m, ql, qr, b, c);
		range_apply(idx << 1 | 1, m, r, ql, qr, b, c);
        pull(idx);
	}

	// range sum on [ql, qr)
	ll range_sum(int idx, int l, int r, int ql, int qr) {
		if (qr <= l || r <= ql) return 0LL;
		if (ql <= l && r <= qr) return seg[idx];
		push(idx, l, r);
		int m = l + r >> 1;
		ll left = range_sum(idx << 1, l, m, ql, qr);
		ll right = range_sum(idx << 1 | 1, m, r, ql, qr);
		ll res = left + right;
		if (res >= MOD) res -= MOD;
		return res;
	}
};

inline void solve() {
	int n, q;
	cin >> n >> q;
	vl a(n);
	for (auto &x : a) { cin >> x; x %= MOD; }

	SegTree st(n);
	st.build(1, 0, n, a);

	while (q--) {
		int type;
		cin >> type;
		if (type == 0) {
			int l, r;
			ll b, c;
			cin >> l >> r >> b >> c;
			b %= MOD;
			c %= MOD;
			st.range_apply(1, 0, n, l, r, b, c);
		} else {
			int l, r;
			cin >> l >> r;
			ll ans = st.range_sum(1, 0, n, l, r);
			ans %= MOD;
			if (ans < 0) ans += MOD;
			cout << ans << '\n';
		}
	}
}

int main() {
    setIO();
    solve(); // single case
}
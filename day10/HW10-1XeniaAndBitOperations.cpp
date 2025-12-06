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
const ll BIG = 4e18; // large enough for ll, not close to INT64_MAX tho
const int MOD = (int)1e9+7; // 998244353;
const int MXN = (int)2e5+5; // modify as needed

struct SegmentTree {
    int n;                  // power-of-two size (number of leaves)
    vector<int> seg;        // segment values
    vector<int> lvl;        // lvl[p] = distance (in edges) from leaves

    // Build base structure and levels
    void init(int _n) {
        n = 1;
        while (n < _n) n <<= 1;
        seg.assign(2 * n, 0);
        lvl.assign(2 * n, 0);

        // leaves: lvl = 0
        for (int i = n; i < 2 * n; ++i) lvl[i] = 0;
        // internal nodes: lvl[parent] = lvl[child] + 1
        for (int i = n - 1; i >= 1; --i) {
            lvl[i] = lvl[i << 1] + 1;
        }
    }

    // combine children a, b at a node with given level
    int cmb(int a, int b, int level) {
        // level 1,3,5,... => OR; level 2,4,6,... => XOR
        if (level & 1) return a | b;
        else return a ^ b;
    }

    void pull(int p) {
        seg[p] = cmb(seg[p << 1], seg[p << 1 | 1], lvl[p]);
    }

    // build from array a[0.._n-1]
    void build(const vector<int>& a) {
        int _n = sz(a);
        init(_n);
        for (int i = 0; i < _n; ++i) {
            seg[n + i] = a[i];
        }
        for (int p = n - 1; p >= 1; --p) {
            pull(p);
        }
    }

    // point update: position pos (1-based in problem) becomes val
    void pointSet(int pos, int val) {
        int p = n + (pos - 1);  // convert to leaf index
        seg[p] = val;
        for (p >>= 1; p >= 1; p >>= 1) {
            pull(p);
        }
    }

    int value() const {
        return seg[1]; // root holds Xenia's v for the whole array
    }
};

inline void solve() {
    int n, m;
    cin >> n >> m;
    int N = 1 << n;
    vector<int> a(N);
    for (int i = 0; i < N; ++i) cin >> a[i];

    SegmentTree st;
    st.build(a);

    while (m--) {
        int p, b;
        cin >> p >> b;
        st.pointSet(p, b);
        cout << st.value() << '\n';
    }
}

int main() {
    setIO();
    solve(); // single case
}
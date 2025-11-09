// Source: https://usaco.guide/general/io
// #pragma GCC optimize("Ofast")
// #pragma GCC target("avx2")

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using lll = __int128; // needed for THIS problem (not all)
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

/*
(1) Problem recap (simple terms):
- d starts at 0. Each "increase" adds +1 to d (cost x), but you cannot do > k increases in a row.
- Each "attack" costs y and deals the current d damage.
- Goal: perform a sequence of increases/attacks to reach total dealt damage >= z with minimum total coins.

(2) Things to notice & step-by-step derivation:
- Let a = total number of "increase" operations, b = total number of "attack" operations.
- Optimal schedule: front-load increments subject to the per-attack cap k. If we set
    c = min(b, a / k),  // number of full k-blocks we can put before early attacks
  then the maximum possible damage with totals (a, b) is
    D(a,b) = k*c*(c+1)/2 + (b - c) * a.
  Reason: first c attacks see cumulative damages k,2k,...,ck; after placing all a increments,
  remaining (b-c) attacks each deal "a".
- For fixed a, D(a,b) increases with b; for fixed b, D(a,b) increases with a.
  So we can binary search the minimal b = opt_b(a) and minimal a = opt_a(b) such that D >= z.

(3) Solution outline:
- Define damage(a,b) via the front-loading closed form (use lll (__int128); clamp not needed).
- Binary search the "crossing" on a: find the smallest a where a >= opt_b(a).
- Enumerate a in [1 .. crossing] and use b = opt_b(a); check cost x*a + y*b.
- Enumerate b in [1 .. crossing] and use a = opt_a(b); check cost again.
- Take the minimal cost. This solves "Monsters" efficiently within the limits.
*/

static inline lll damage128(ll a, ll b, ll k) {
	// c = number of full k-blocks we can place before early attacks
	ll c = min(b, (k == 0 ? 0 : a / k)); // k>=1 in constraints, keep guard anyway
	// D = k * c * (c+1) / 2 + (b - c) * a
	lll tri = (lll)k * (lll)c * (lll)(c + 1) / 2;
	lll tail = (lll)(b - c) * (lll)a;
	return tri + tail;
}

inline void solve() {
    ll x, y, z, k; cin >> x >> y >> z >> k;

    auto cost = [&](ll a, ll b)->lll {
        return (lll)a * x + (lll)b * y;
    };

    // minimal a such that D(a, b) >= z (monotone in a)
    auto opt_a = [&](ll b)->ll {
        ll lo = 1, hi = z + 1; // a=0 yields D=0, so start at 1
        while (lo < hi) {
            ll mid = (lo + hi) >> 1;
            if (damage128(mid, b, k) >= (lll)z) hi = mid;
            else lo = mid + 1;
        }
        return lo; // in [1..z+1]
    };

    // minimal b such that D(a, b) >= z (monotone in b)
    auto opt_b = [&](ll a)->ll {
        ll lo = 1, hi = z + 1; // b=0 yields D=0, so start at 1
        while (lo < hi) {
            ll mid = (lo + hi) >> 1;
            if (damage128(a, mid, k) >= (lll)z) hi = mid;
            else lo = mid + 1;
        }
        return lo; // in [1..z+1]
    };

    // Find the "crossing" a* where a* >= opt_b(a*)
    ll lo = 0, hi = z;
    while (lo < hi) {
        ll a = (lo + hi) >> 1;
        (a >= opt_b(a)) ? hi = a : lo = a + 1;
    }
    // lo is the smallest a with a >= opt_b(a); 1 <= lo <= z
    ll cross = max<ll>(1, min<ll>(z, lo));

    lll ans = (lll)4e36; // big

    // Sweep a up to crossing
    for (ll a = 1; a <= cross; ++a) {
        ll b = opt_b(a);
        if (b <= z) ans = min(ans, cost(a, b));
    }
    // Sweep b up to crossing
    for (ll b = 1; b <= cross; ++b) {
        ll a = opt_a(b);
        if (a <= z) ans = min(ans, cost(a, b));
    }

    // Print lll (__int128) safely
    if (ans == 0) { cout << 0 << '\n'; return; }
    bool neg = ans < 0; if (neg) ans = -ans;
    string s;
    while (ans) { int d = (int)(ans % 10); s.pb('0' + d); ans /= 10; }
    if (neg) s.pb('-');
    reverse(all(s));
    cout << s << '\n';
}

int main() {
	setIO();
    ll t; cin >> t;
    while(t--) solve();
}

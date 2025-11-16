/* Source: https://usaco.guide/general/io */
// #pragma GCC optimize("Ofast")
// #pragma GCC target("avx2")

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cmath>
#include <complex>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <vector>
#include <climits>
#include <type_traits>
#include <utility>
#include <sstream>
#include <iterator>

using namespace std;
 
using ll = long long;
using db = long double; // or double, if TL is tight
using str = string; // yay python! 

// pairs
using pi = pair<int,int>;
using pl = pair<ll,ll>;
using pd = pair<db,db>;
#define mp make_pair
#define f first
#define s second

#define tcT template<class T
#define tcTU tcT, class U
tcT> using V = vector<T>; 
tcT, size_t SZ> using AR = array<T,SZ>; 
using vi = V<int>;
using vb = V<bool>;
using vl = V<ll>;
using vd = V<db>;
using vs = V<str>;
using vpi = V<pi>;
using vpl = V<pl>;
using vpd = V<pd>;

// vectors
#define sz(x) int((x).size())
#define bg(x) begin(x)
#define all(x) bg(x), end(x)
#define rall(x) x.rbegin(), x.rend() 
#define sor(x) sort(all(x)) 
#define rsz resize
#define ins insert 
#define pb push_back
#define eb emplace_back
#define ft front()
#define bk back()

#define lb lower_bound
#define ub upper_bound
tcT> int lwb(V<T>& a, const T& b) { return int(lb(all(a),b)-bg(a)); }
tcT> int upb(V<T>& a, const T& b) { return int(ub(all(a),b)-bg(a)); }

// loops
#define FOR(i,a,b) for (int i = (a); i < (b); ++i)
#define F0R(i,a) FOR(i,0,a)
#define ROF(i,a,b) for (int i = (b)-1; i >= (a); --i)
#define R0F(i,a) ROF(i,0,a)
#define rep(a) F0R(_,a)
#define each(a,x) for (auto& a: x)

const int MOD = (int)1e9+7; // 998244353;
const int MX = (int)2e5+5;
const ll BIG = 1e18; // not too close to LLONG_MAX
const db PI = acos((db)-1);
const int dx[4]{1,0,-1,0}, dy[4]{0,1,0,-1};
mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count()); 
template<class T> using pqg = priority_queue<T,vector<T>,greater<T>>;

// bitwise ops
constexpr int pct(int x) { return __builtin_popcount(x); }
constexpr int bits(int x) {
	return x == 0 ? 0 : 31-__builtin_clz(x); }
constexpr int p2(int x) { return 1<<x; }
constexpr int msk2(int x) { return p2(x)-1; }

ll cdiv(ll a, ll b) { return a/b+((a^b)>0&&a%b); }
ll fdiv(ll a, ll b) { return a/b-((a^b)<0&&a%b); }

tcT> bool ckmin(T& a, const T& b) {
	return b < a ? a = b, 1 : 0; }
tcT> bool ckmax(T& a, const T& b) {
	return a < b ? a = b, 1 : 0; }

tcTU> T fstTrue(T lo, T hi, U f) {
	++hi; assert(lo <= hi);
	while (lo < hi) {
		T mid = lo+(hi-lo)/2;
		f(mid) ? hi = mid : lo = mid+1; 
	} 
	return lo;
}
tcTU> T lstTrue(T lo, T hi, U f) {
	--lo; assert(lo <= hi);
	while (lo < hi) {
		T mid = lo+(hi-lo+1)/2;
		f(mid) ? lo = mid : hi = mid-1;
	} 
	return lo;
}
tcT> void remDup(vector<T>& v) {
	sort(all(v)); v.erase(unique(all(v)),end(v)); }
tcTU> void erase(T& t, const U& u) {
	auto it = t.find(u); assert(it != end(t));
	t.erase(it); }

inline namespace FileIO {
	void setIn(str s)  { freopen(s.c_str(),"r",stdin); }
	void setOut(str s) { freopen(s.c_str(),"w",stdout); }
	void setIO(str s = "") {
		cin.tie(0)->sync_with_stdio(0);
		cin.exceptions(cin.failbit);
		if (sz(s)) setIn(s+".in"), setOut(s+".out");
	}
}

////////////////////////////////////////////////////////////////////

/*
 1) Problem recap in simple terms:
    We have N points with x>0, y>0 and the origin (0,0). We must pick a subset of
    these points plus the origin that forms a simple strictly convex polygon
    (no 180° angles / consecutive parallel edges), and we want the maximum number
    of vertices. The origin must be one of the vertices.

 2) Things to notice and step-by-step derivation:
    - All given points lie in the first quadrant, so their polar angles around (0,0)
      are in (0, pi/2). In any convex polygon that uses the origin and only these
      points, the other vertices appear in strictly increasing order of polar angle.
    - Any valid polygon therefore looks like:
          0 -> p[i1] -> p[i2] -> ... -> p[ik] -> 0
      with angle(0, p[i1]) < angle(0, p[i2]) < ... < angle(0, p[ik]).
    - Convexity is local: for every triple of consecutive vertices A, B, C on the
      boundary, we must have 0 < angle ABC < 180°, i.e. cross(B-A, C-B) has a
      constant non-zero sign (all left turns).
    - Once points are sorted by angle, we can search for the longest convex chain
      in that order that starts at 0 and ends back at 0. This suggests a DP on the
      last two vertices of the chain.

 3) Solution outline:
    - Read N points, store as pairs (x,y), then sort by increasing polar angle
      around the origin, breaking ties by distance from the origin.
    - Let p[0] = (0,0), and p[1..N] be the sorted points.
    - Define dp[i][j] (0 <= j < i <= N) as the maximum number of vertices on a
      convex chain
          0 -> ... -> j -> i
      respecting the angle order.
      Base: dp[i][0] = 2 for all i (chain 0 -> i).
    - Transition:
        for every k < j < i, we can append i after j in a chain ending with k -> j
        iff cross( p[j] - p[k], p[i] - p[j] ) > 0
        (strictly convex left turn).
        Then:
            dp[i][j] = max(dp[i][j], dp[j][k] + 1).
    - To close the polygon, for each pair (j, i) we check if we can connect
      i back to 0 while keeping convexity at i:
          cross( p[i] - p[j], (0,0) - p[i] ) > 0.
      Whenever this holds and dp[i][j] is valid, the chain + closing edge gives a
      convex polygon with dp[i][j] vertices. Take the maximum over all pairs.
    - Complexity is O(N^3) with N <= 100, which is easily fast enough.
*/

// geometric types and helpers.
using T = db; // or ll
const T EPS = 1e-9;
using P = pair<T,T>; using vP = V<P>; using Line = pair<P,P>;
int sgn(T a) { return (a>EPS)-(a<-EPS); }
T sq(T a) { return a*a; }

bool close(const P& a, const P& b) { 
	return sgn(a.f-b.f) == 0 && sgn(a.s-b.s) == 0; } 
T abs2(const P& p) { return sq(p.f)+sq(p.s); }
T abs(const P& p) { return sqrt(abs2(p)); }
T arg(const P& p) { return atan2(p.s,p.f); }
P conj(const P& p) { return P(p.f,-p.s); }
P perp(const P& p) { return P(-p.s,p.f); }
P dir(T ang) { return P(cos(ang),sin(ang)); }

P operator-(const P& l) { return P(-l.f,-l.s); }
P operator+(const P& l, const P& r) { 
	return P(l.f+r.f,l.s+r.s); }
P operator-(const P& l, const P& r) { 
	return P(l.f-r.f,l.s-r.s); }
P operator*(const P& l, const T& r) { 
	return P(l.f*r,l.s*r); }
P operator*(const T& l, const P& r) { return r*l; }
P operator/(const P& l, const T& r) { 
	return P(l.f/r,l.s/r); }
P operator*(const P& l, const P& r) { 
	return P(l.f*r.f-l.s*r.s,l.s*r.f+l.f*r.s); }
P operator/(const P& l, const P& r) { 
	return l*conj(r)/abs2(r); }
P& operator+=(P& l, const P& r) { return l = l+r; }
P& operator-=(P& l, const P& r) { return l = l-r; }
P& operator*=(P& l, const T& r) { return l = l*r; }
P& operator/=(P& l, const T& r) { return l = l/r; }
P& operator*=(P& l, const P& r) { return l = l*r; }
P& operator/=(P& l, const P& r) { return l = l/r; }

P unit(const P& p) { return p/abs(p); }
T dot(const P& a, const P& b) { return a.f*b.f+a.s*b.s; }
T dot(const P& p, const P& a, const P& b) { return dot(a-p,b-p); }
T cross(const P& a, const P& b) { return a.f*b.s-a.s*b.f; }
T cross(const P& p, const P& a, const P& b) {
	return cross(a-p,b-p); }
P reflect(const P& p, const Line& l) {
	P a = l.f, d = l.s-l.f;
	return a+conj((p-a)/d)*d; }
P foot(const P& p, const Line& l) {
	return (p+reflect(p,l))/(T)2; }
bool onSeg(const P& p, const Line& l) {
	return sgn(cross(l.f,l.s,p)) == 0 && sgn(dot(p,l.f,l.s)) <= 0; }

// make hull, get size of hull
inline void solve() {
    int n; cin >> n;
    vP pts(n);
	each (pt, pts) cin >> pt.f >> pt.s; // input x, y (ints, but stored as db)

	// Sort points by polar angle around origin, then by distance.
	sort(all(pts), [](const P& a, const P& b) {
		T aa = arg(a), bb = arg(b);
		int s = sgn(aa - bb);
		if (s < 0) return true;
		if (s > 0) return false;
		// same angle: nearer first
		return abs2(a) < abs2(b);
	});

	// p[0] = origin, p[1..n] = sorted points
	vP p;
	p.pb(P(0, 0));
	each (pt, pts) p.pb(pt);

	// dp[i][j] : max vertices in chain 0 -> ... -> j -> i
	// 0 <= j < i <= n
	V<vi> dp(n + 1, vi(n + 1, INT32_MIN));

	int ans = 3; // problem guarantees result >= 3

	FOR (i, 1, n + 1) {
		// Base: chain 0 -> i (two vertices)
		dp[i][0] = 2;

		FOR (j, 1, i) {
			int best = INT32_MIN;

			// Try all previous vertices k before j
			F0R (k, j) {
				if (dp[j][k] <= 0) continue; // unreachable state

				P v1 = p[j] - p[k]; // k -> j
				P v2 = p[i] - p[j]; // j -> i

				// Strictly convex left turn at j
				if (sgn(cross(v1, v2)) > 0) ckmax(best, dp[j][k] + 1);
			}

			dp[i][j] = best;

			// Try to close chain 0 -> ... -> j -> i -> 0 if this state is valid
			if (dp[i][j] > 0) {
				P v1 = p[i] - p[j];    // j -> i
				P v2 = P(0, 0) - p[i]; // i -> 0

				if (sgn(cross(v1, v2)) > 0) ckmax(ans, dp[i][j]);
			}
		}
	}

	cout << ans << '\n';
}

int main() {
	setIO();
    solve(); // single case
}
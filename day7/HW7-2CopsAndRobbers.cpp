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
 problem recap in simple terms:
 - We have multiple datasets with positions of cops, robbers, and citizens on a 2D plane.
 - A citizen is "safe" if they lie inside (or on the edge of) some triangle formed by three cops.
 - If not safe, a citizen is "robbed" if they lie inside (or on the edge of) some triangle formed by three robbers.
 - Otherwise the citizen is "neither".

 things to notice and step-by-step derivation:
 - The union of all triangles from a point set equals the region of its convex hull (for non-degenerate sets).
 - So instead of checking all triples of cops/robbers, it is enough to:
   - Build the convex hull of cops and robbers.
   - Do a point-in-polygon test for each citizen against those hulls.
 - Degenerate hulls need care:
   - Fewer than 3 original points → cannot form any triangle.
   - All points on one spot → only that point is covered.
   - All points collinear → only the segment between the extremes is covered.

 solution outline:
 - Read c, r, o; stop when all are zero.
 - Read all cop, robber, and citizen coordinates.
 - Build convex hull for cops and robbers.
 - For each citizen:
   - If inside cops' region → "safe".
   - Else if inside robbers' region → "robbed".
   - Else → "neither".
 - Print the required formatted output for each dataset.
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

// point-in-polygon: 0 = on edge, -1 = inside, 1 = outside.
int inPoly(const P& p, const vP& poly) {
	int n = sz(poly), ans = 0;
	F0R(i,n) {
		P x = poly[i], y = poly[(i+1)%n]; 
		if (x.s > y.s) swap(x,y);
		if (onSeg(p,{x,y})) return 0;
		ans ^= (x.s <= p.s && p.s < y.s && cross(p,x,y) > 0);
	}
	return ans ? -1 : 1;
}

// convex hull helpers.
pair<vi,vi> ulHull(const vP& v) {
	vi p(sz(v)), u, l; iota(all(p), 0);
	sort(all(p), [&v](int a, int b) { return v[a] < v[b]; });
	each(i,p) {
		#define ADDP(C, cmp) while (sz(C) > 1 && cross(\
			v[C[sz(C)-2]],v[C.bk],v[i]) cmp 0) C.pop_back(); C.pb(i);
		ADDP(u, >=); ADDP(l, <=);
	}
	return {u,l};
}
vi hullInd(const vP& v) { // returns indices in CCW order
	vi u,l; tie(u,l) = ulHull(v); if (sz(l) <= 1) return l;
	if (v[l[0]] == v[l[1]]) return {0};
	l.insert(end(l),1+rall(u)-1); return l;
}

// build the hull.
vP hull(const vP& v) {
	vi w = hullInd(v); vP res; each(t,w) res.pb(v[t]);
	return res;
}

// check if pt is inside union of all triangles from this set.
bool insideSet(const P& pt, const vP& hull, int originalCount) {
    if (originalCount < 3 || hull.empty()) return false;
    if (sz(hull) == 1) {
        // >=3 points all at same location.
        return close(pt, hull[0]);
    }
    if (sz(hull) == 2) {
        // all points collinear: region is the segment between extremes.
        Line L{hull[0], hull[1]};
        return onSeg(pt, L);
    }
    // normal convex hull region.
    return inPoly(pt, hull) <= 0; // inside or on boundary.
}

int c, r, o;
static int tc = 1;

// process one dataset: classify each citizen as safe/robbed/neither.
inline void solve() {
    vP cops(c), robbers(r), citizens(o);
    each(x,cops) cin >> x.f >> x.s;
    each(x,robbers) cin >> x.f >> x.s;
    each(x,citizens) cin >> x.f >> x.s;

    vP copsHull = hull(cops);
    vP robbersHull = hull(robbers);

    vs statuses(o);
    F0R(i,o) {
        if (insideSet(citizens[i], copsHull, c)) statuses[i] = "safe";
        else if (insideSet(citizens[i], robbersHull, r)) statuses[i] = "robbed";
        else statuses[i] = "neither";
    }

    cout << "Data set " << tc << ":\n";
    F0R(i,o) {
        cout << "     Citizen at (" << citizens[i].f << ',' << citizens[i].s
             << ") is " << statuses[i] << ".\n";
    }
    cout << '\n';
}

// main loop: read datasets until "0 0 0".
int main() {
	setIO();
    for (;;) {
        if (!(cin >> c >> r >> o)) return 0;
        if (!c && !r && !o) break;
        solve();
        tc++;
    }
}
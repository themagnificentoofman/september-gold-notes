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
		cin.exceptions(cin.failbit); // comment out if breaks things
		if (sz(s)) setIn(s+".in"), setOut(s+".out");
	}
}

// constants (add more if needed)
const db PI = acos((db)-1);
const ll BIG = 4e18; // large enough for ll, not close to INT64_MAX tho
const int MOD = (int)1e9+7; // 998244353;
const int MXN = (int)2e5+5; // modify as needed

// GNU PBDS: ordered set (supports order_of_key / find_by_order)
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template<class T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

/*
 (1) Problem recap in simple terms:
     We are given up to t test cases. In each test we have an array A of n distinct positive integers.
     We must count how many pairs (i, j) with i < j have A[i] > A[j] (the inversion count).

 (2) Things to notice and step-by-step derivation:
     - n can be as large as 200,000, so an O(n^2) double loop would be too slow.
     - We need an O(n log n) approach per test case.
     - If we scan the array from left to right and maintain all previous elements in a structure that can count
       "how many previous elements are greater than current A[i]", we can sum those counts to get the total inversions.
     - An ordered_set from GNU PBDS supports:
           order_of_key(x) = number of elements strictly less than x,
       so the number of elements strictly greater than x among k elements is:
           k - order_of_key(x).
     - Because the array elements are distinct, we don’t need to worry about duplicates.

 (3) Solution outline:
     - For each test case:
         * Read n and the n array elements.
         * Maintain an ordered_set<int> s representing all elements seen so far as we scan left to right.
         * For each x in the array:
               inv += s.size() - s.order_of_key(x);   // number of previous elements > x
               s.insert(x);
         * Print inv as a 64-bit integer (long long).
     - Overall complexity: O(n log n) per test case, which fits the constraints.
*/

inline void solve() {
	int n;
	cin >> n;

	ordered_set<int> s; // keeps the elements we have seen so far
	ll inv = 0;

	for (int i = 0; i < n; i++) {
		int x;
		cin >> x;

		// number of previous elements greater than x:
		// s.size() elements total, of which order_of_key(x) are < x,
		// and since all numbers are distinct and x not yet inserted, this equals strictly greater than x.
		inv += (ll)sz(s) - (ll)s.order_of_key(x);

		s.ins(x);
	}

	cout << inv << '\n';
}

int main() {
	setIO();
	int t; cin >> t;
	while (t--) solve();
}

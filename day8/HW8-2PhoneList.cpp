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
		// cin.exceptions(cin.failbit); // comment out if breaks things
		if (sz(s)) setIn(s+".in"), setOut(s+".out");
	}
}

// constants (add more if needed)
const db PI = acos((db)-1);
const ll BIG = 4e18; // large enough for ll
const int MOD = (int)1e9+7; // 998244353;
const int MXN = (int)2e5+5; // not used here

/*
 (1) problem recap in simple terms:
     We have several test cases. Each test case has a list of phone numbers.
     The list is consistent if no number is a prefix of another number in the same test case.
     For each test, we output "YES" if consistent and "NO" otherwise.

 (2) things to notice and step-by-step derivation:
     - If we sort all phone numbers lexicographically, any prefix pair must appear next to each other.
     - So instead of checking all pairs (O(n^2)), we only need to compare each number with the next one in sorted order (O(n log n)).
     - For two adjacent strings a and b (in sorted order), check if a is a prefix of b:
       length(a) <= length(b) and b.compare(0, length(a), a) == 0.
     - If we find such a pair, the list is inconsistent.

 (3) solution outline:
     - For each test case:
       * Read n and then read the n phone numbers into a vector<string>.
       * Sort the vector.
       * Scan once over the sorted vector, checking each adjacent pair for a prefix relation.
       * If any prefix is found, print "NO", otherwise print "YES".
*/

inline void solve() {
	int n; cin >> n;

	vs nums(n);
	for (int i = 0; i < n; i++) cin >> nums[i];

	sor(nums); // lexicographic sort

	bool ok = true;
	for (int i = 0; i + 1 < n && ok; i++) {
		const str& a = nums[i];
		const str& b = nums[i + 1];

		if (sz(a) <= sz(b)) {
			// check if a is prefix of b
			if (b.compare(0, sz(a), a) == 0) ok = false;
		}
	}

	cout << (ok ? "YES" : "NO") << '\n';
}

int main() {
	setIO();
	int t = 1; cin >> t;
	while (t--) solve();
}
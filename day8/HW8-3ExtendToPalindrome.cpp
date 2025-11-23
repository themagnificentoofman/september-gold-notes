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
     For each input string s, we must append the fewest characters at the end of s so that
     the result is a palindrome, and print that palindrome.

 (2) things to notice and step-by-step derivation:
     - Let sr = reverse(s). If we solve "shortest palindrome by adding characters to the
       FRONT" for sr, then reversing that result gives the shortest palindrome by adding
       characters to the END for s.
     - The classic KMP trick for "add to front" is:
         * Build t = x + '#' + reverse(x) (here x = sr).
         * Compute the prefix-function (pi array) for t.
         * pi.back() is the length L of the longest palindromic prefix of x.
         * The shortest palindrome is reverse(x.substr(L)) + x.
     - Translating back to s:
         * x = sr, reverse(x) = s.
         * Final palindrome for s is s + sr.substr(L).

 (3) solution outline:
     - For each line s:
       * Compute sr = reverse(s).
       * Build t = sr + '#' + s and its prefix-function pi.
       * Let L = pi.back().
       * Answer = s + sr.substr(L).
     - Process until EOF and print one result per line.
*/

// builds the prefix-function (KMP pi array) for a string
vi build_prefix(const str& t) {
	int n = sz(t);
	vi pi(n);
	for (int i = 1; i < n; i++) {
		int j = pi[i - 1];
		while (j > 0 && t[i] != t[j]) j = pi[j - 1];
		if (t[i] == t[j]) j++;
		pi[i] = j;
	}
	return pi;
}

// constructs the minimal palindrome by appending characters to the end of s
str make_palindrome(const str& s) {
	str sr = s;
	reverse(all(sr));          // sr = reverse(s)
	str t = sr + '#' + s;      // separator '#' is safe (only letters in input)
	vi pi = build_prefix(t);
	int L = pi.back();         // longest palindromic prefix length of sr
	return s + sr.substr(L);   // append minimal extra part
}

int main() {
	setIO();
	str s;
	while (cin >> s) {
		str ans = make_palindrome(s);
		cout << ans << '\n';
	}
}
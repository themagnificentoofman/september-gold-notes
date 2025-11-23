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
     We get many test cases. Each has a pattern (needle) and a text (haystack).
     For each test case, we must print all starting positions where the pattern appears in the text, 0-based.
     Between outputs of different test cases we print a blank line (even if one test has no matches).

 (2) things to notice and step-by-step derivation:
     - Lengths can be large, so a naive O(n * m) search may TLE.
     - KMP gives us O(n + m) by precomputing the prefix-function (failure function) for the pattern.
     - While scanning the haystack once, we maintain current matched prefix length and update using the prefix-function.
     - Each time the matched prefix length equals pattern length, we found a match; record its starting index and continue.
     - Input ends at EOF; for each block: read length, then pattern line, then haystack line.
     - For formatting, print a blank line before every test case output except the first one.

 (3) solution outline:
     - Implement a function to build the prefix-function for the pattern.
     - Implement a KMP search function that returns a vector<int> of match starting positions.
     - In main(), read test cases until reading the pattern length fails (EOF).
     - For each test case, run KMP, handle blank lines between test cases, and print all positions (one per line).
*/

// builds prefix-function (pi array) for pattern
vi build_prefix(const str& pat) {
	int m = sz(pat);
	vi pi(m, 0);
	int j = 0;
	for (int i = 1; i < m; i++) {
		while (j > 0 && pat[i] != pat[j]) j = pi[j - 1];
		if (pat[i] == pat[j]) j++;
		pi[i] = j;
	}
	return pi;
}

// KMP search: returns all starting positions where pat occurs in text
vi kmp_find(const str& pat, const str& text) {
	vi res;
	int m = sz(pat), n = sz(text);
	if (m == 0 || n == 0 || m > n) return res;

	vi pi = build_prefix(pat);
	int j = 0; // current matched prefix length in pat

	for (int i = 0; i < n; i++) {
		while (j > 0 && text[i] != pat[j]) j = pi[j - 1];
		if (text[i] == pat[j]) j++;
		if (j == m) {
			// match ends at i, so it starts at i - m + 1
			res.pb(i - m + 1);
			j = pi[j - 1]; // continue searching for next match
		}
	}
	return res;
}

int main() {
	setIO();
	bool firstCase = true;
	for (;;) {
		int L;
		if (!(cin >> L)) break; // EOF

		str needle, haystack;
		cin >> ws;               // eat newline/whitespace after L
		getline(cin, needle);    // read needle line
		getline(cin, haystack);  // read haystack line

		vi positions = kmp_find(needle, haystack);

		// print blank line between different test cases
		if (!firstCase) cout << '\n';
		firstCase = false;

		for (int pos : positions) cout << pos << '\n';
	}
}

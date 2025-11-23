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
     We have two strings A and B of length N. We can "shift" B left (move first char to end),
     any number of times. For each possible number of shifts k (0..N-1), we look at the rotated
     B_k and measure how long the common prefix with A is. We must choose the k that gives the
     longest common prefix; if there are ties, pick the smallest k.

 (2) things to notice and step-by-step derivation:
     - Rotating B by k is the same as taking the substring of (B + B) starting at position k
       with length N.
     - The longest common prefix between A and B_k is just the longest prefix match between A
       and the substring (B + B)[k..].
     - If we build S = A + '#' + (B + B) and run the Z-algorithm on S, then Z[i] tells us
       the longest prefix of A that matches S starting at position i.
       For the starting position k in (B + B), the corresponding index in S is idx = |A| + 1 + k.
     - For each k in [0..N-1], we get LCP_k = min(Z[idx], N). We track the maximum LCP, and on
       ties choose the smallest k.

 (3) solution outline:
     - Read N, A, B.
     - Build BB = B + B and S = A + '#' + BB.
     - Run Z-algorithm on S to get array z.
     - For each k from 0 to N-1:
         * idx = |A| + 1 + k
         * cur = min(z[idx], N)
         * keep the k with largest cur; on ties keep smaller k.
     - Output the best k.
*/

// standard Z-algorithm: z[i] = LCP length of s[i..] with s[0..]
vi build_z(const str& s) {
	int n = sz(s);
	vi z(n, 0);
	int l = 0, r = 0;
	for (int i = 1; i < n; i++) {
		if (i <= r) z[i] = min(r - i + 1, z[i - l]);
		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
		if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
	}
	return z;
}

inline void solve() {
	int N; str A, B;
	cin >> N >> A >> B;

	str BB = B + B;
	str S;
	S.reserve(sz(A) + 1 + sz(BB));
	S = A + '#' + BB;

	vi z = build_z(S);

	int bestShift = 0;
	int bestLen = -1;
	int lenA = sz(A);

	for (int k = 0; k < N; k++) {
		int idx = lenA + 1 + k; // starting position of BB[k] in S
		int cur = z[idx];
		if (cur > N) cur = N;   // LCP can't exceed N
		if (cur > bestLen) {
			bestLen = cur;
			bestShift = k;
		}
		// if cur == bestLen, we keep the existing smaller k
	}

	cout << bestShift << '\n';
}

int main() {
	setIO();
	solve(); // single case
}
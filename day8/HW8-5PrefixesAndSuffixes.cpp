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
 (1) problem recap in simple terms
     We are given a string s. For every prefix that is also a suffix of s, we must
     output its length and how many times that prefix appears as a substring in s.
 (2) things to notice and step-by-step derivation
     The prefix-function (KMP pi-array) lets us find all "borders" (prefix == suffix)
     by repeatedly jumping with pi from the full length down to 0. Also, if we count
     how often each pi value appears and then propagate these counts from longer prefixes
     to their own borders, we get how many times each prefix occurs inside the string (as a substring).
 (3) solution outline
     Compute the prefix-function pi[] in O(n). Build an array cnt[] where we first count
     occurrences of each pi value, then propagate counts from longer to shorter using pi,
     and finally add 1 for the prefix itself. Then collect all border lengths by following
     L = n, L = pi[L-1], ... until 0, reverse this list to get increasing order,
     and print each length with cnt[length].
*/

inline void solve() {
    str s;
    if (!(cin >> s)) return;
    int n = sz(s);

    // compute prefix-function pi[i] = length of longest proper prefix
    // which is also suffix for s[0..i]
    vi pi(n, 0);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j])
            j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }

    // cnt[len] will eventually store the number of occurrences
    // of prefix of length len as a substring
    vi cnt(n + 1, 0);

    // count how many times each length appears as a pi-value
    for (int i = 0; i < n; i++)
        cnt[pi[i]]++;

    // propagate counts: occurrences of longer prefixes contribute
    // to their own borders (shorter prefixes)
    for (int i = n - 1; i > 0; i--)
        cnt[pi[i - 1]] += cnt[i];

    // each prefix occurs at least once (the prefix itself)
    for (int i = 1; i <= n; i++)
        cnt[i]++; // add the "own" occurrence

    // collect all lengths that are both prefix and suffix
    vi lens;
    int L = n;
    while (L > 0) {
        lens.pb(L);
        L = pi[L - 1]; // jump to next border
    }
    sor(lens); // increasing order

    cout << sz(lens) << '\n';
    for (int len : lens)
        cout << len << ' ' << cnt[len] << '\n';
}

int main() {
    setIO();
    solve(); // single case
}
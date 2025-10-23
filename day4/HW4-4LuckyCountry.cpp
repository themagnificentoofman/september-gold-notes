// Source: https://usaco.guide/general/io
// #pragma GCC optimize("Ofast")
// #pragma GCC target("avx2")

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
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
const db PI = acos((db)-1);
const int MOD = (int)1e9+7; // 998244353;
const int MX = (int)2e5+5; // change as needed

/*
 Approach:
 - Find sizes of connected components via DSU.
 - Let freq[s] = number of components of size s.
 - Binary split each freq[s] into sums of powers of two: c = 1,2,4,...,rem.
   Each split creates a 0/1 item with weight = s * c and cost = c (components used).
 - DP: dp[x] = min number of components to reach total size x. Initialize dp[0]=0, else INF.
 - Generate all lucky numbers <= n.
 - Answer = min over lucky L of (dp[L] - 1). If no L reachable, output -1.
*/

struct DSU {
	vi p, r;
	DSU(int n=0){init(n);}
	void init(int n){
		p.rsz(n+1), r.rsz(n+1,1);
		iota(all(p),0);
	}
	int f(int x){ return p[x]==x ? x : p[x]=f(p[x]); }
	bool unite(int a,int b){
		a=f(a); b=f(b);
		if(a==b) return false;
		if(r[a]<r[b]) swap(a,b);
		r[a]+=r[b]; p[b]=a;
		return true;
	}
};

inline void gen_lucky(int n, vi &v) {
	// DFS to generate all lucky numbers up to n
	function<void(ll)> dfs = [&](ll cur){
		if(cur>n) return;
		if(cur>0) v.pb((int)cur);
		dfs(cur*10+4);
		dfs(cur*10+7);
	};
	dfs(0);
}

inline void solve() {
	int n,m; 
	if(!(cin>>n>>m)) return;
	DSU dsu(n);
	for(int i=0;i<m;i++){ int u,v; cin>>u>>v; dsu.unite(u,v); }

	// Collect and get component sizes
	vi compSize(n+1,0);
	for(int i=1;i<=n;i++) if(dsu.f(i)==i) compSize[dsu.r[dsu.f(i)]]++;

	// Prepare items via binary splitting: each item = (weight, cost)
	V<pi> items;
	for(int s=1;s<=n;s++){
		int cnt = compSize[s];
		if(!cnt) continue;
		for(int p=1; p<=cnt; p<<=1){
			items.pb({s*p, p});
			cnt -= p;
		}
		if(cnt) items.pb({s*cnt, cnt});
	}

	// dp[sum] = min # of components to reach sum
	vi dp(n+1, INT32_MAX); dp[0]=0;
	for(auto [w,c]: items){
		for(int j=n-w; j>=0; --j){
			if(dp[j]==INT32_MAX) continue;
			int &to = dp[j+w];
			int cand = dp[j] + c;
			if(cand < to) to = cand;
		}
	}

	// Generate lucky numbers and get ans
	vi lucky; gen_lucky(n, lucky);
	int ans = INT32_MAX;
	for(int L: lucky){
		if(dp[L] < INT32_MAX) ans = min(ans, dp[L]-1);
	}
	cout << (ans==INT32_MAX ? -1 : ans) << '\n';
}

int main() {
	setIO();
	solve();
}
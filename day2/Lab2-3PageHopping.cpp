/* Source: https://usaco.guide/general/io */

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using db = long double;
template<class T> using V = vector<T>;
template<class T1, class T2> using P = pair<T1, T2>;
#define bg(x) begin(x)
#define all(x) bg(x), end(x)
#define sz(x) (int)(x).size()
#define pb push_back
#define fi first
#define se second
using vi = V<int>;
using pi = P<int,int>;
inline namespace FileIO {
	void setIn(string s){freopen(s.c_str(),"r",stdin);}
	void setOut(string s){freopen(s.c_str(),"w",stdout);}
	void setIO(string s = ""){
		cin.tie(0)->sync_with_stdio(0);
		cin.exceptions(cin.failbit);
		if (sz(s)) setIn(s+".in"), setOut(s+".out");
	}
}
const db PI = acos((db)-1);

constexpr ll INF = (1LL<<60);
ll dist[101][101];

static inline void init_dist() {
	for (int i = 1; i <= 100; ++i)
		for (int j = 1; j <= 100; ++j)
			dist[i][j] = (i == j ? 0 : INF);
}

int main() {
	setIO();
    int cs = 1;
    while(true) {
        init_dist();
        int a, b;
		if (!(cin >> a >> b)) break;            // EOF -> done.
		if (a == 0 && b == 0) break;            // no more cases.

		dist[a][b] = 1;
		while (cin >> a >> b) {
			if (a == 0 && b == 0) break;        // end of this case.
			dist[a][b] = 1;
		}

		// Floyd–Warshall with overflow guard.
		for (int k = 1; k <= 100; ++k)
			for (int i = 1; i <= 100; ++i)
				if (dist[i][k] < INF)
					for (int j = 1; j <= 100; ++j)
						if (dist[k][j] < INF) {
							ll cand = dist[i][k] + dist[k][j];
							if (cand < dist[i][j]) dist[i][j] = cand;
						}

		// Average over reachable ordered pairs (i != j).
		ll sum = 0;
		int num = 0;
		for (int i = 1; i <= 100; ++i)
			for (int j = 1; j <= 100; ++j)
				if (i != j && dist[i][j] < INF) {
					sum += dist[i][j];
					++num;
				}

		cout << "Case " << cs++ << ": average length between pages = "
		     << fixed << setprecision(3) << ((db)sum / num) << " clicks\n";
    }
}
// Source: https://usaco.guide/general/io
// #pragma GCC optimize("Ofast")

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

/*
 Dijkstra on expanded state space:
  - dist[u][f] = minimum cost to be at city u with f fuel.
  - Transitions:
      buy:   (u,f) -> (u,f+1) with cost +price[u], if f < cap
      drive: (u,f) -> (v,f-d) with cost +0, if edge u-v of length d and f >= d
  - Early exit: first time we pop city == e, that's optimal (any fuel).
*/

int main() {
	setIO();

	int n, m;
	if (!(cin >> n >> m)) return 0;

	vi price(n);
	for (int i = 0; i < n; ++i) cin >> price[i];

	/* Adjacency list: for each city, store (neighbor, distance). */
	V<V<pi>> adj(n);
	for (int i = 0; i < m; ++i) {
		int u, v, d; 
		cin >> u >> v >> d;
		adj[u].pb({v, d});
		adj[v].pb({u, d});
	}

	int q; 
	cin >> q;

	/*
	 Answer each query independently, because capacity (state space width) changes.
	*/
	while (q--) {
		int cap, s, e;
		cin >> cap >> s >> e;

		/*
		 dist[u][f] initialized to INF, size n x (cap+1)
		*/
		const ll INF = (ll)4e18;
		V<V<ll>> dist(n, V<ll>(cap + 1, INF));

		/*
		 Min-heap of (cost, city, fuel).
		 We use tuple for clarity.
		*/
		using State = tuple<ll,int,int>;
		priority_queue<State, V<State>, greater<State>> pq;

		// Start at (s, 0 fuel) with 0 cost.
		dist[s][0] = 0;
		pq.push({0LL, s, 0});

		/*
		 Dijkstra loop.
		*/
		bool found = false;
		while (!pq.empty()) {
            ll cost;
            int u, f;
            tie(cost, u, f) = pq.top();
            pq.pop();

			// Skip if we already found a better way to this state.
			if (cost > dist[u][f]) continue;

			// Early exit: first time we pop any state at city e, it's optimal.
			if (u == e) {
				cout << cost << '\n';
				found = true;
				break;
			}

			/*
			 1) Option to buy +1 unit of fuel at city u (if not at capacity).
			    Cost increases by price[u].
			*/
			if (f < cap) {
				ll ncost = cost + price[u];
				if (ncost < dist[u][f + 1]) {
					dist[u][f + 1] = ncost;
					pq.push({ncost, u, f + 1});
				}
			}

			/*
			 2) Option to drive along any adjacent edge (u -> v) of length d,
			    if we have enough fuel.
			    Driving consumes fuel but costs no money at the moment of travel.
			*/
			for (auto neighbor : adj[u]) {
                int v = neighbor.fi, d = neighbor.se;
				if (d <= f) {
					if (cost < dist[v][f - d]) {
						dist[v][f - d] = cost;
						pq.push({cost, v, f - d});
					}
				}
			}
		}

		// If we exhausted the queue without popping a state at e, it's impossible.
		if (!found) cout << "impossible\n";
	}
}
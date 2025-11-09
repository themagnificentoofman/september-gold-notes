// Source: https://usaco.guide/general/io
// #pragma GCC optimize("Ofast")
// #pragma GCC target("avx2")

#include <bits/stdc++.h>
using namespace std;

// basic type aliases
using str = string;
using ll = long long;
// using i128 = __int128_t; // not used here (commented out just in case UVa doesn't like it)
using db = long double;

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
        // cin.exceptions(cin.failbit); // comment out if this ever causes issues
        if (sz(s)) setIn(s+".in"), setOut(s+".out");
    }
}

// constants
const db PI = acos((db)-1);
const ll BIG = (ll)4e18;

/*
 (1) Problem recap in simple terms:
     - You drive through a desert from distance 0 to some goal distance.
     - Along the way, events change the situation: fuel consumption rate changes, leaks appear,
       leaks are fixed, you hit gas stations, and finally reach the goal.
     - Your tank capacity must be large enough that, starting full and refilling to full
       at each gas station, you never run out of fuel before the goal.
     - We need to compute the minimal such tank capacity.

 (2) Things to notice and step-by-step derivation:
     - Fuel usage over a segment of length d (km) is:
         * (current_fuel_consumption_per_100km / 100) * d
         * plus (number_of_leaks * 1 litre/km) * d
     - Capacity is only "tested" between refills:
         * At the very start we can assume we begin with a full tank.
         * At a gas station we refill back to full.
         * Between two refills, we simply accumulate fuel used along the driven distance.
     - If we simulate the journey once and track:
         * used = fuel used since last refill,
         * best = maximum value of used seen at any refill or at the goal,
       then best is exactly the minimum tank capacity needed.

 (3) Solution outline:
     - Repeatedly read test cases until we see the sentinel line "0 Fuel consumption 0".
     - For each test case:
         * Read the first line "0 Fuel consumption n" to get initial consumption.
         * Initialize:
             - current_consumption = n (litres per 100 km),
             - leaks = 0,
             - used = 0 (fuel used since last refill),
             - best = 0 (max fuel usage encountered so far),
             - prev_dist = 0 (last event position).
         * Read subsequent events in order. For each event at distance d:
             1) Let delta = d - prev_dist.
             2) Increase used by:
                    used += (current_consumption / 100.0) * delta
                            + leaks * 1.0 * delta.
             3) Update prev_dist = d.
             4) Process the event type:
                - "Fuel consumption x": update current_consumption = x.
                - "Leak": leaks++.
                - "Mechanic": leaks = 0.
                - "Gas station": best = max(best, used); used = 0 (we refill tank).
                - "Goal": best = max(best, used); print best with 3 decimals and
                  stop reading this test case.
     - Print one line (with 3 digits after decimal point) per test case.
*/

// Simulate one full test case starting from the line "0 Fuel consumption n" that we already read.
// Parameters:
//  - startDist = 0 for this problem (but we pass it for clarity).
//  - initialConsumption = the n from "Fuel consumption n".
void solve(int startDist, int initialConsumption) {
    // Current state variables.
    double currentConsumption = initialConsumption; // litres per 100 km.
    int leaks = 0;                                  // number of active leaks.
    double used = 0.0;                              // fuel used since last refill.
    double best = 0.0;                              // maximum needed capacity so far.
    int prevDist = startDist;                       // last distance where an event was processed.

    for (;;) {
        int d;
        string word;
        cin >> d >> word;

        int delta = d - prevDist;
        // Consume fuel along this segment.
        used += (currentConsumption / 100.0) * delta;
        used += (double)leaks * (double)delta;
        prevDist = d;

        if (word == "Fuel") {
            // "Fuel consumption x"
            string tmp;
            int x;
            cin >> tmp >> x; // tmp == "consumption"
            currentConsumption = x;
        } else if (word == "Leak") {
            // A new leak: increases loss by 1 litre/km.
            leaks++;
        } else if (word == "Gas") {
            // "Gas station": we refill tank to full here.
            string tmp;
            cin >> tmp; // tmp == "station"
            if (used > best) best = used;
            used = 0.0; // reset usage after refill.
        } else if (word == "Mechanic") {
            // Mechanic fixes all leaks.
            leaks = 0;
        } else if (word == "Goal") {
            // Journey ends.
            if (used > best) best = used;
            cout << fixed << setprecision(3) << best << '\n';
            break;
        }
    }
}

int main() {
    setIO();
    /*
      We repeatedly read the first line of each test case:
        distance (must be 0), "Fuel", "consumption", n.
      Sentinel: "0 Fuel consumption 0" means end of input.
    */
    for (;;) {
        int dist;
        string w1;
        cin >> dist >> w1;

        string w2;
        int n;
        cin >> w2 >> n; // w2 == "consumption"

        // Sentinel check.
        if (dist == 0 && n == 0) break;

        // Process this test case starting from this first "Fuel consumption n".
        solve(dist, n);
    }
}
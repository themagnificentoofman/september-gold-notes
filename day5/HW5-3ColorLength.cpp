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
 (1) Problem recap in simple terms:
     We have two lanes of cars, each lane is a string of colors (A..Z). We must merge
     them into one lane while keeping the relative order inside each lane.
     
     After merging, for every color c:
         L(c) = last_position(c) - first_position(c)
     and we want to minimize sum_c L(c).
     Output that minimum for each test.

 (2) Things to notice and step-by-step derivation:
     Key observation:
     - For a given merged sequence, consider time t = 1..(n+m-1).
       Look at the set of "open" colors after placing position t:
         * A color is "open" if we've already seen it at/before t,
           BUT we will also see it again later.
       Then sum_c L(c) = sum over t of (# of open colors at time t).

     Why is that true?
     - For any color c, its contribution L(c) = last_c - first_c.
       That equals the number of positions t where c has started
       but not finished yet (i.e. t is between first_c and last_c-1).
       Summing over colors just sums, for each t, how many colors
       are still "active" across t.

     This means:
     When we append the next car to our merged string, the cost we
     add is exactly "how many colors are currently open after
     appending that car."

     DP design:
     Let DP[i][j] = minimum total cost after merging the first i cars
     from lane A and first j cars from lane B (so length = i+j).
     The last car we placed could be:
        - A[i]  (came from state DP[i-1][j])
        - B[j]  (came from state DP[i][j-1])
     After placing that last car, we pay "openCount(i,j)" where
     openCount(i,j) = number of colors that have appeared in the
     already-picked prefix of length i+j and will also appear again
     in the remaining suffix (not yet picked).

     Recurrence:
        DP[i][j] = min(
            DP[i-1][j] + openCount(i,j),
            DP[i][j-1] + openCount(i,j)
        )
     with DP[0][0] = 0.

     Computing openCount(i,j) fast:
     - For each prefix length i of A and j of B:
         seenMask(i,j): which colors have ALREADY appeared in A[0..i-1] or B[0..j-1].
         remMask(i,j):  which colors will STILL appear in A[i..end] or B[j..end].
       Then any color in both sets is "open".
       So:
          openMask(i,j) = seenMask(i,j) & remMask(i,j)
          openCount(i,j) = popcount(openMask(i,j))

     We can precompute:
         prefA[i]  = bitmask of colors in A[0..i-1]
         prefB[j]  = bitmask of colors in B[0..j-1]
         sufA[i]   = bitmask of colors in A[i..n-1]
         sufB[j]   = bitmask of colors in B[j..m-1]
       Then:
         seenMask(i,j) = prefA[i] | prefB[j]
         remMask(i,j)  = sufA[i]  | sufB[j]

     popcount fits in int since only 26 colors.

 (3) Solution outline:
     - Precompute prefix masks and suffix masks for both strings.
     - Do DP with rolling arrays to save memory:
         prev[j] = DP[i-1][j]
         cur[j]  = DP[i][j]
       Transition:
         cur[0]  = prev[0] + openCount(i,0)
         cur[j]  = min(prev[j], cur[j-1]) + openCount(i,j)

     Complexity:
         Time  O(n * m)
         Memory O(m)
         n,m ≤ 5000 so n*m ≤ 25e6, which is fine.
*/

static inline int colorBit(char ch) {
    return 1 << (ch - 'A'); // 'A'..'Z' -> bit 0..25.
}

int solve(const string &A, const string &B) {
    int n = sz(A), m = sz(B);

    /* prefix masks:
       prefA[i] = colors seen in A[0..i-1]    (length i prefix)
       prefB[j] = colors seen in B[0..j-1]
    */
    vi prefA(n+1), prefB(m+1);
    prefA[0] = 0;
    for (int i = 0; i < n; i++) {
        prefA[i+1] = prefA[i] | colorBit(A[i]);
    }
    prefB[0] = 0;
    for (int j = 0; j < m; j++) {
        prefB[j+1] = prefB[j] | colorBit(B[j]);
    }

    /* suffix masks:
       sufA[i] = colors in A[i..n-1]  (what's still remaining if we've already
                                       consumed first i cars from A)
       sufB[j] = colors in B[j..m-1]
    */
    vi sufA(n+1), sufB(m+1);
    sufA[n] = 0;
    for (int i = n-1; i >= 0; i--) {
        sufA[i] = sufA[i+1] | colorBit(A[i]);
    }
    sufB[m] = 0;
    for (int j = m-1; j >= 0; j--) {
        sufB[j] = sufB[j+1] | colorBit(B[j]);
    }

    /* DP running rows:
       prev[j] = DP[i-1][j]
       cur[j]  = DP[i][j]
    */
    vi prev(m+1, INT32_MAX), cur(m+1, INT32_MAX);

    // Base DP[0][0] = 0.
    prev[0] = 0;

    // Fill first row i=0 (taking only from B).
    for (int j = 1; j <= m; j++) {
        int seenMask = prefA[0] | prefB[j];
        int remMask  = sufA[0]  | sufB[j];
        int openCnt  = __builtin_popcount(seenMask & remMask);

        prev[j] = prev[j-1] + openCnt;
    }

    // Fill rows i=1..n.
    for (int i = 1; i <= n; i++) {
        // j = 0 (taking only from A so far).
        {
            int seenMask = prefA[i] | prefB[0];
            int remMask  = sufA[i]  | sufB[0];
            int openCnt  = __builtin_popcount(seenMask & remMask);

            cur[0] = prev[0] + openCnt;
        }

        // j = 1..m (mixing A[0..i-1] and B[0..j-1]).
        for (int j = 1; j <= m; j++) {
            int seenMask = prefA[i] | prefB[j];
            int remMask  = sufA[i]  | sufB[j];
            int openCnt  = __builtin_popcount(seenMask & remMask);

            // DP[i][j] = min(DP[i-1][j], DP[i][j-1]) + openCnt
            cur[j] = min(prev[j], cur[j-1]) + openCnt;
        }

        // Move cur -> prev for next row.
        swap(prev, cur);
    }

    // Answer is DP[n][m] = prev[m].
    return prev[m];
}

int main() {
    setIO();
    int t; cin >> t;
    while (t--) {
        string a, b;
        cin >> a >> b;
        cout << solve(a, b) << '\n';
    }
}
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
#define f first
#define s second
using vi = V<int>;
using pi = P<int,int>;
inline namespace FileIO {
	void setIn(string s){freopen(s.c_str(),"r",stdin);}
	void setOut(string s){freopen(s.c_str(),"w",stdout);}
	void setIO(string s = ""){
		cin.tie(0)->sync_with_stdio(0);
		// cin.exceptions(cin.failbit);
		if (sz(s)) setIn(s+".in"), setOut(s+".out");
	}
}
const db PI = acos((db)-1);

vi vis, indeg;
V<vi> adj;

int main() {
    setIO();
    string varsLine, consLine;
    bool firstCase = true;
    auto tokenize = [](const string &str) {
        V<string> t;
        string x;
        stringstream ss(str);
        while (ss >> x) t.pb(x);
        return t;
    };
    while(true) {
        varsLine.clear();
        // read variables line, skip blank lines
        while(getline(cin, varsLine)) {
            bool onlySpaces = true;
            for(const char &c : varsLine) if(!isspace((unsigned char)c)) { onlySpaces = false; break; }
            if(!onlySpaces) break;
        }
        if(!cin) break; // EOF
        consLine.clear();
        if(!getline(cin, consLine)) consLine.clear();
        V<string> vtokens = tokenize(varsLine);
        V<char> letters;
        letters.reserve(sz(vtokens));
        for(const string &t : vtokens) if(!t.empty()) letters.pb(t[0]);
        sort(all(letters));
        letters.erase(unique(all(letters)), end(letters));
        int n, m;
        n = sz(letters);
        m = 0;
        unordered_map<char,int> id;
        for(int i = 0; i < n; i++) id[letters[i]] = i;
        adj.assign(n, vi());
        vis.assign(n, 0);
        indeg.assign(n, 0);
        V<string> ctokens = tokenize(consLine);
        V<V<bool>> hasEdge(n, V<bool>(n));
        for(int i = 0; i < sz(ctokens)-1; i += 2) {
            char a = ctokens[i][0];
            char b = ctokens[i+1][0];
            if(!id.count(a) || !id.count(b)) continue;
            int u = id[a], v = id[b];
            if(u != v && !hasEdge[u][v]) {
                hasEdge[u][v] = true;
                adj[u].pb(v);
                indeg[v]++;
                m++;
            }
        }
        vi indeg_cur = indeg;
        V<string> output;
        string cur; cur.reserve(n);
        function<void()> dfs = [&]() {
            if(sz(cur) == n) {
                output.pb(cur);
                return;
            }
            for(int i = 0; i < n; ++i) {
                if(!vis[i] && indeg_cur[i] == 0) {
                    vis[i] = 1;
                    cur.pb(letters[i]);
                    for(const int &v : adj[i]) indeg_cur[v]--;
                    dfs();
                    for(const int &v : adj[i]) indeg_cur[v]++;
                    cur.pop_back();
                    vis[i] = 0;
                }
            }
        };
        dfs();
        if(!firstCase) cout << "\n";
        firstCase = false;
        for(const string &str : output) cout << str << "\n";
    }
}
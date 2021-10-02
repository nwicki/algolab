#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> graph;
int target;

const int MAX = ((1L << 31) - 1);
const int MIN = 1 << 31;

int holmes(int r, int b, bool even);
int moriarty(int r, int b, bool even);
vector<vector<int>> memo_holmes_red;
vector<vector<int>> memo_holmes_black;
vector<vector<int>> memo_moriarty_red;
vector<vector<int>> memo_moriarty_black;

int holmes(int r, int b, bool even) {
    int result = 1;
    if(even) {
        if(r == target + 1) {
            return 0;
        }
        int mem = memo_holmes_red[r][b];
        if(mem != -1) {
            return mem;
        }
        for(auto transition : graph[r]) {
            result = min(result, moriarty(transition, b, even));
        }
        memo_holmes_red[r][b] = result;
    }
    else {
        if(b == target + 1) {
            return 1;
        }
        int mem = memo_holmes_black[r][b];
        if(mem != -1) {
            return mem;
        }
        for(auto transition : graph[b]) {
            result = min(result, moriarty(r, transition, even));
        }
        memo_holmes_black[r][b] = result;
    }
    return result;
}

int moriarty(int r, int b, bool even) {
    int result = 0;
    if(even) {
        even = !even;
        if(b == target + 1) {
            return 1;
        }
        int mem = memo_moriarty_black[r][b];
        if(mem != -1) {
            return mem;
        }
        for(auto transition : graph[b]) {
            result = max(result, holmes(r, transition, even));
        }
        memo_moriarty_black[r][b] = result;
    }
    else {
        even = !even;
        if(r == target + 1) {
            return 0;
        }
        int mem = memo_moriarty_red[r][b];
        if(mem != -1) {
            return mem;
        }
        for(auto transition : graph[r]) {
            result = max(result, holmes(transition, b, even));
        }
        memo_moriarty_red[r][b] = result;
    }
    return result;
}

void compute_75(int r, int b, int n) {
    memo_holmes_red = vector<vector<int>>(n,vector<int>(n,-1));
    memo_holmes_black = vector<vector<int>>(n,vector<int>(n,-1));
    memo_moriarty_red = vector<vector<int>>(n,vector<int>(n,-1));
    memo_moriarty_black = vector<vector<int>>(n,vector<int>(n,-1));
    cout << holmes(r,b,true) << endl;
}

int move_even(int m);
int move_odd(int m);
vector<int> memo_even;
vector<int> memo_odd;

int move_even(int m) {
    if(m == target) {
        return 0;
    }
    int memo = memo_even[m];
    if(memo != -1) {
        return memo;
    }
    int result = MAX;
    for(int transition : graph[m]) {
        result = min(result, move_odd(transition));
    }
    result++;
    memo_even[m] = result;
    return result;
}

int move_odd(int m) {
    if(m == target) {
        return 0;
    }
    int memo = memo_odd[m];
    if(memo != -1) {
        return memo;
    }
    int result = MIN;
    for(int transition : graph[m]) {
        result = max(result, move_even(transition));
    }
    result++;
    memo_odd[m] = result;
    return result;
}

void compute_100(int r, int b, int n) {
    memo_even = vector<int>(n,-1);
    memo_odd = vector<int>(n,-1);
    int holmes_count = move_even(r);
    int moriarty_count = move_even(b);
    if(holmes_count == moriarty_count) {
        cout << !(holmes_count % 2) << endl;
        return;
    }
    cout << (moriarty_count < holmes_count) << endl;
}

void testcase() {
    int n,m,r,b; cin >> n >> m >> r >> b;
    target = n - 1, r--, b--;
    graph = vector<vector<int>>(n,vector<int>());
    for(int i = 0; i < m; i++) {
        int u,v; cin >> u >> v;
        graph[u-1].push_back(v-1);
    }
    compute_100(r,b,n);
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for (int i = 0; i < t; ++i) {
        testcase();
    }
    return 0;
}
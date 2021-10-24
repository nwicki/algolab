#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

// 1 3 6 2 1 2 1 2 1 2 2 2

using namespace std;

class csr {
public:
    int cycle;
    int strengths;
    bool reset;
    csr(int c, int s, bool r) {
        cycle = c;
        strengths = s;
        reset = r;
    }
};

class cmp_csr {
public:
    bool operator()(const csr& a, const csr& b) {
      return a.cycle > b.cycle;
    }
};

void testcase() {
    int n, m; cin >> n >> m;
    vector<int> strengths(n);
    for(int& s : strengths) {
        cin >> s;
    }
    sort(strengths.begin(), strengths.end(), greater<>());
    vector<int> weights(m);
    for(auto& w : weights) {
        cin >> w;
    }
    sort(weights.begin(), weights.end(), greater<>());
    if(strengths.front() < weights.front()) {
        cout << "impossible" << endl;
        return;
    }
    vector<pair<int,int>> strengths_weights;
    pair<int,int> current(0, 0);
    auto its = strengths.begin(), itw = weights.begin();
    while(its != strengths.end() && itw != weights.end()) {
        while(its != strengths.end() && *itw <= *its) {
            current.first++;
            its++;
        }
        while (itw != weights.end() && *its < *itw) {
            current.second++;
            itw++;
        }
        strengths_weights.push_back(current);
        current = make_pair(0, 0);
    }
    if(itw != weights.end() || its != strengths.end()) {
        while(its != strengths.end()) {
            current.first++;
            its++;
        }
        while (itw != weights.end()) {
            current.second++;
            itw++;
        }
        strengths_weights.push_back(current);
    }
    int cycles = 0;
    priority_queue<csr, vector<csr>, cmp_csr> remaining;
    for(const auto& sw : strengths_weights) {
        int w = sw.second;
        int c = 0;
        remaining.push(csr(0, sw.first, true));
        while(w != 0) {
            while(w != 0 && remaining.top().cycle == c) {
                auto top = remaining.top();
                remaining.pop();
                if(top.reset) {
                    remaining.push(csr(top.cycle+1, top.strengths, true));
                }
                if(top.strengths <= w) {
                    w -= top.strengths;
                }
                else {
                    remaining.push(csr(top.cycle, top.strengths - w, false));
                    w = 0;
                }
            }
            c++;
        }
        cycles = max(cycles,c);
    }
    cout << ((3*cycles)-1) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
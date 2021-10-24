#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

// 1 3 6 2 1 2 1 2 1 2 2 2

using namespace std;

bool cmp_weights(const pair<int,bool>& a, const pair<int,bool>& b) {
    return a.first > b.first;
}

void test1() {
    int n, m; cin >> n >> m;
    vector<int> strengths(n);
    for(int& s : strengths) {
        cin >> s;
    }
    sort(strengths.begin(), strengths.end(), greater<>());
    vector<pair<int,bool>> weights(m);
    for(auto& w : weights) {
        cin >> w.first;
        w.second = true;
    }
    sort(weights.begin(), weights.end(), cmp_weights);
    if(strengths.front() < weights.front().first) {
        cout << "impossible" << endl;
        return;
    }
    int time = 0;
    int count = 0;
    while(count != m) {
        int i = 0;
        int j = 0;
        while(i < n && j < m) {
            if(weights[j].second) {
                if(weights[j].first <= strengths[i]) {
                    weights[j].second = false;
                    count++;
                    i++;
                    j++;
                }
                else {
                    j++;
                }
            }
            else {
                j++;
            }
        }
        time += 3;
    }
    time--;
    cout << time << endl;
}

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

void test2() {
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
    vector<pair<int,int>> condensed_strengths;
    pair<int,int> current_strength = make_pair(0,strengths.front());
    for(const auto& s : strengths) {
        if(s == current_strength.second) {
            current_strength.first++;
        }
        else {
            condensed_strengths.push_back(current_strength);
            current_strength = make_pair(1,s);
        }
    }
    condensed_strengths.push_back(current_strength);
    vector<pair<int,int>> condensed_weights;
    pair<int,int> current_weight = make_pair(0,weights.front());
    for(const auto& w : weights) {
        if(w == current_weight.second) {
            current_weight.first++;
        }
        else {
            condensed_weights.push_back(current_weight);
            current_weight = make_pair(1, w);
        }
    }
    condensed_weights.push_back(current_weight);
    vector<int> strengths_to_weights(condensed_weights.size(),0);
    for(size_t i = 0, j = 0; i < condensed_weights.size(); i++) {
        int strength_amount = 0;
        while(j < condensed_strengths.size() && condensed_weights[i].second <= condensed_strengths[j].second) {
            strength_amount += condensed_strengths[j].first;
            j++;
        }
        strengths_to_weights[i] = strength_amount;
    }
    vector<pair<int,int>> sws;
    pair<int,int> current_sws(0,0);
    for(auto its = strengths.begin(), itw = weights.begin(); its != strengths.end() || itw != weights.end();) {
        while(its != strengths.end() && *itw <= *its) {
            current_sws.first++;
            its++;
        }
        if(its == strengths.end()) {
            while (itw != weights.end()) {
                current_sws.second++;
                itw++;
            }
        }
        else {
            while (itw != weights.end() && *its < *itw) {
                current_sws.second++;
                itw++;
            }
        }
        sws.push_back(current_sws);
        current_sws = make_pair(0,0);
    }
    vector<pair<int,int>> strengths_weights;
    pair<int,int> current_sw = make_pair(strengths_to_weights[0],condensed_weights[0].first);
    for(size_t i = 1; i < strengths_to_weights.size(); i++) {
        if(strengths_to_weights[i] != 0) {
            strengths_weights.push_back(current_sw);
            current_sw = make_pair(0,0);
        }
        current_sw.first += strengths_to_weights[i];
        current_sw.second += condensed_weights[i].first;
    }
    strengths_weights.push_back(current_sw);
    int cycles = 0;
    priority_queue<csr, vector<csr>, cmp_csr> remaining;
    for(const auto& sw : sws) {
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
        test2();
    }
}
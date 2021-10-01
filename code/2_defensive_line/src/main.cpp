#include <iostream>
#include <vector>

using namespace std;

void compute_40(const vector<int>& defenders, int n, int m, int k) {
    vector<pair<int,int>> intervals;
    int sum = 0;
    int start = 0;
    for(int i = 0; i < n; i++) {
        sum += defenders[i];
        if(sum == k) {
            intervals.emplace_back(start,i);
        }
        else if(k < sum) {
            while(k < sum && start < i) {
                sum -= defenders[start];
                start++;
            }
            if(sum == k) {
                intervals.emplace_back(start,i);
            }
        }
    }
    if(intervals.size() < m) {
        cout << "fail" << endl;
        return;
    }
    if(m == 1) {
        int max = 0;
        for(auto e : intervals) {
            int captured = e.second - e.first + 1;
            if(max < captured) {
                max = captured;
            }
        }
        cout << max << endl;
        return;
    }
    if(m == 2) {
        int max = 0;
        bool not_found = true;
        for(int i = 0; i < intervals.size(); i++) {
            auto ii = intervals[i];
            int captured_i = ii.second - ii.first + 1;
            for(int j = i + 1; j < intervals.size(); j++) {
                auto ij = intervals[j];
                if(ii.second < ij.first) {
                    not_found = false;
                    int captured = captured_i + ij.second - ij.first + 1;
                    if(max < captured) {
                        max = captured;
                    }
                }
            }
        }
        if(not_found) {
            cout << "fail" << endl;
            return;
        }
        cout << max << endl;
        return;
    }
    cout << "not yet handled" << endl;
}

void prune(vector<pair<int,int>>& intervals) {
    int reach = -1;
    for(auto it = intervals.begin(); it != intervals.end();) {
        if(reach < it->first) {
            reach = it->second;
            it++;
        }
        else {
            intervals.erase(it);
        }
    }
}

class Interval {
public:
    int start;
    int end;
    int range;
    Interval(int s,int e) {
        start = s;
        end = e;
        range = end-start+1;
    }
};

bool compare_interval(Interval i, Interval j) {
    return i.range > j.range;
}

void compute_60(const vector<int>& defenders, int n, int m, int k) {
    vector<Interval> intervals;
    int sum = 0;
    int start = 0;
    for(int i = 0; i < n; i++) {
        sum += defenders[i];
        if(sum == k) {
            intervals.emplace_back(start,i);
        }
        else if(k < sum) {
            while(k < sum && start < i) {
                sum -= defenders[start];
                start++;
            }
            if(sum == k) {
                intervals.emplace_back(start,i);
            }
        }
    }
    if(intervals.size() < m) {
        cout << "fail" << endl;
        return;
    }
    if(m == 1) {
        cout << intervals[0].range << endl;
        return;
    }
    if(m == 2) {
        vector<vector<Interval>> result(intervals.size(),vector<Interval>());
        for(int i = 0; i < intervals.size(); i++) {
            auto ii = intervals[i];
            result[i].push_back(ii);
        }
        cout << "fail" << endl;
        return;
    }
    cout << "not yet handled" << endl;
}

void testcase() {
    int n,m,k; cin >> n >> m >> k;
    vector<int> defenders(n);
    for(int& e : defenders) {
        cin >> e;
    }
    compute_60(defenders,n,m,k);
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
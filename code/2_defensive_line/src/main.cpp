#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

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
vector<Interval> intervals;

void compute_40(const vector<int>& defenders, int n, int m, int k) {
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
    if((int) intervals.size() < m) {
        cout << "fail" << endl;
        return;
    }
    if(m == 1) {
        int max = 0;
        for(auto e : intervals) {
            int captured = e.range;
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
        int size = (int) intervals.size();
        for(int i = 0; i < size; i++) {
            auto ii = intervals[i];
            int captured_i = ii.range;
            for(int j = i + 1; j < size; j++) {
                auto ij = intervals[j];
                if(ii.end < ij.start) {
                    not_found = false;
                    int captured = captured_i + ij.range;
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

const int MIN = 1 << 31;
vector<vector<int>> memo;
vector<int> max_interval;
vector<int> starts;

void set_intervals(const vector<int>& defenders, int n, int k) {
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
}

void set_starts(int size) {
    starts = vector<int>(size);
    int next_start = 1;
    for(int i = 0; i < size; i++) {
        int end = intervals[i].end;
        while(next_start < size && intervals[next_start].start <= end) {
            next_start++;
        }
        if(size <= next_start) {
            starts[i] = -1;
        }
        else {
            starts[i] = next_start;
        }
    }
}

void set_max_interval(int size) {
    max_interval = vector<int>(size);
    max_interval[size-1] = intervals[size-1].range;
    for(int i = size-2; -1 < i; i--) {
        max_interval[i] = max(max_interval[i+1],intervals[i].range);
    }
}

int find_matches(int start, int m) {
    if(m == 0) {
        return 0;
    }
    if(starts[start] == -1) {
        return MIN;
    }
    int mem = memo[m][start];
    if(mem != -1) {
        return mem;
    }
    int result = MIN;
    if(m == 1) {
        result = max_interval[starts[start]];
    }
    else {
        int rem_m = (int) intervals.size() - m + 1;
        for(int i = starts[start]; i < rem_m; i++) {
            int current = find_matches(i,m-1);
            if(-1 < current) {
                result = max(result,intervals[i].range + current);
            }
        }
    }
    memo[m][start] = result;
    return result;
}

void compute_100(const vector<int>& defenders, int n, int m, int k) {
    set_intervals(defenders,n,k);
    int size = (int) intervals.size();
    if(size < m) {
        cout << "fail" << endl;
        return;
    }
    set_starts(size);
    set_max_interval(size);
    memo = vector<vector<int>>(m+1,vector<int>(size,-1));
    int rem_m = size - m + 1;
    int result = MIN;
    if(m == 1) {
        result = max_interval[0];
    }
    else {
        for(int i = 0; i < rem_m; i++) {
            int current = find_matches(i,m-1);
            if(-1 < current) {
                result = max(result,intervals[i].range + current);
            }
        }
    }
    if(result < 0) {
        cout << "fail" << endl;
        return;
    }
    cout << result << endl;
}

void testcase() {
    int n,m,k; cin >> n >> m >> k;
    vector<int> defenders(n);
    for(int& e : defenders) {
        cin >> e;
    }
    intervals.clear();
    compute_100(defenders,n,m,k);
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
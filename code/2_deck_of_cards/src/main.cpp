#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

void sum_trigger(int k, int sum, int start, int i, pair<int,int>& interval, int& min) {
    int diff = abs(k-sum);
    if(diff < min) {
        interval = make_pair(start,i);
        min = diff;
    }
}

void testcase() {
    int n,k; cin >> n >> k;
    auto interval = make_pair(0,0);
    int min = (1L << 31) - 1;
    int sum = 0;
    vector<int> v(n);
    int start = 0;
    for(int i = 0; i < n; i++) {
        cin >> v[i];
        if(min != 0) {
            sum += v[i];
            sum_trigger(k,sum,start,i,interval,min);
            while(k < sum && start < i) {
                sum -= v[start];
                start++;
            }
            sum_trigger(k,sum,start,i,interval,min);
        }
    }
    cout << interval.first << " " << interval.second << endl;
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
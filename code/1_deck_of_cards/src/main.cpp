#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class Interval {
public:
    int start;
    int end;
    int sum;
    Interval(int s, int e, int su) {
        start = s;
        end = e;
        sum = su;
    }
};


// 1 ≤ n ≤ 10^5, 0 ≤ k ≤ 2^30, sum of deck ≤ 2^30
void testcase() {
    int n,k; cin >> n >> k;
    vector<int> deck(n);
    Interval opt = Interval(0,0,2<<30);
    Interval current = Interval(0,0,0);
    for(int i = 0; i < n; i++) {
        int v;
        cin >> v;
        current.sum += v;
        current.end = i;
        deck[i] = v;
        while(k < current.sum) {
            current.sum -= deck[current.start++];
        }
        if(abs(current.sum - k) < abs(opt.sum - k)) {
            opt = current;
        }
    }
    cout << opt.start << " " << opt.end << endl;
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
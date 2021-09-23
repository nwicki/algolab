#include <iostream>

using namespace std;

// 1 ≤ n ≤ 10^6, 1 ≤ h_i < 2^31
void testcase() {
    int n,h; cin >> n >> h;
    int sum = h-1;
    int count = 1;
    for(int i = 1; i < n; i++) {
        cin >> h;
        if(0 < sum) {
            sum--;
            count++;
            sum = max(h-1,sum);
        }
    }
    cout << count << endl;
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
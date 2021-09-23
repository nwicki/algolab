#include <iostream>

using namespace std;

void testcase() {
    int n; cin >> n;
    int a;
    int sum = 0;
    for(int i = 0; i < n; i++) {
        cin >> a;
        sum += a;
    }
    cout << sum << endl;
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
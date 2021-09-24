#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool cmp(int a, int b) {
    return a > b;
}

void testcase() {
    int n; cin >> n;
    vector<int> uector(n);
    for(int i = 0; i < n; i++) {
        cin >> uector[i];
    }
    int x; cin >> x;
    if(x) {
        sort(uector.begin(),uector.end(),cmp);
    }
    else {
        sort(uector.begin(),uector.end());
    }
    for(int i : uector) {
        cout << i << " ";
    }
    cout << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 0; i < t; ++i) {
        testcase();
    }
    return 0;
}
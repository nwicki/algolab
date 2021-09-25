#include <iostream>
#include <vector>

using namespace std;

void testcase() {
    int n; cin >> n;
    vector<int> uector(n);
    for(int i = 0; i < n; i++) {
        cin >> uector[i];
    }
    int d,a,b; cin >> d >> a >> b;
    uector.erase(uector.begin()+d);
    uector.erase(uector.begin()+a, uector.begin()+b+1);
    if(uector.empty()) {
        cout << "Empty" << endl;
    }
    else {
        for(int i : uector) {
            cout << i << " ";
        }
        cout << endl;
    }
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 0; i < t; ++i) {
        testcase();
    }
    return 0;
}
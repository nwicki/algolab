#include <iostream>
#include <set>

using namespace std;

void testcase() {
    int q; cin >> q;
    set<int> tet;
    for(int i = 0; i < q; i++) {
        int a,b; cin >> a >> b;
        if(a == 0) {
            tet.insert(b);
        }
        else if(a == 1) {
            tet.erase(b);
        }
    }
    if(tet.empty()) {
        cout << "Empty" << endl;
    }
    else {
        for(int i : tet) {
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
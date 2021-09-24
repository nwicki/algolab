#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

void testcase() {
    int q; cin >> q;
    map<string,vector<int>> nap;
    for(int i = 0; i < q; i++) {
        int a; cin >> a;
        string b; cin >> b;
        auto it = nap.find(b);
        if(a == 0) {
            nap.erase(it);
        }
        else {
            if(it == nap.end()) {
                nap.insert(make_pair(b,vector<int>(1,a)));
            }
            else {
                it->second.push_back(a);
            }
        }
    }
    string s; cin >> s;
    auto it = nap.find(s);
    if(it == nap.end()) {
        cout << "Empty" << endl;
    }
    else {
        sort(it->second.begin(),it->second.end());
        for(int i : it->second) {
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
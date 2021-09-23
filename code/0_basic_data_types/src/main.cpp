#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

void testcase() {
    int i; cin >> i;
    long l; cin >> l;
    string s; cin >> s;
    double d; cin >> d;
    cout << setprecision(2) << fixed;
    cout << i << " " << l << " " << s << " " << d << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 0; i < t; ++i) {
        testcase();
    }
    return 0;
}
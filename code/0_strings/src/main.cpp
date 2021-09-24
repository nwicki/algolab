#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

void testcase() {
    string a,b; cin >> a >> b;
    cout << a.size() << " " << b.size() << endl;
    cout << a << b << endl;
    reverse(a.begin(),a.end());
    reverse(b.begin(),b.end());
    char temp = a[0];
    a[0] = b[0];
    b[0] = temp,
    cout << a << " " << b << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 0; i < t; ++i) {
        testcase();
    }
    return 0;
}
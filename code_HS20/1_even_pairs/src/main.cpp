#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

void compute_even_pairs_1(const vector<int>& array) {
    int n = (int) array.size();
    int count = 0;
    for(int i = 0; i < n; i++) {
        int sum = 0;
        for(int j = i; j < n; j++) {
            sum += array[j];
            count += sum % 2 == 0;
        }
    }
    cout << count << endl;
}

void compute_even_pairs_2(const vector<int>& array) {
    int n = (int) array.size();
    int count = 0;
    int even = 0;
    int total_even = 0;
    int odd = 0;
    for(int i = 0; i < n; i++) {
            if(array[i] % 2 == 0) {
                even++;
                total_even++;
                count += even;
            }
            else {
                even = 0;
                odd++;
                if(odd % 2 == 0) {
                    count += odd / 2;
                    count += total_even;
                }
                else {
                    count += odd / 2;
                }
            }
    }
    cout << count << endl;
}

void compute_even_pairs_3(const vector<int>& array) {
    int n = (int) array.size();
    vector<int> sums(n,0);
    int count = 0;
    int even = 0;
    int sum = array[0];
    even += sum % 2 == 0;
    sums[0] = sum;
    for(int i = 1; i < n; i++) {
        sum = array[i] + sums[i-1];
        even += sum % 2 == 0;
        sums[i] = sum;
    }
    int odd =  n - even;
    count += (even - 1) * even / 2;
    count += (odd - 1) * odd / 2;
    count += even;
    cout << count << endl;
}

void testcase() {
    int n; cin >> n;
    vector<int> array(n);
    for(int& x : array) {
        cin >> x;
    }
    compute_even_pairs_3(array);
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 0; i < t; ++i) {
        testcase();
    }
    return 0;
}
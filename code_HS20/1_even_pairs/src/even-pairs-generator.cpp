//
// Created by Nicolas Wicki on 21.09.21.
//
#include <iostream>
#include <cmath>


using namespace std;

int main(int argc, char const *argv[]) {
    int l; cin >> l;
    int testcases = 0;
    for(int i = 1; i <= l; i++) {
        testcases += pow(2,i);
    }
    cout << testcases << endl;
    for(int i = 1; i <= l; i++) {
        int cases = pow(2,i);
        for(int j = 0; j < cases; j++) {
            cout << i << endl;
            for(int k = pow(2,i-1); k != 0; k/=2) {
                cout << !(!(k & j)) << " ";
            }
            cout << endl;
        }
    }
}
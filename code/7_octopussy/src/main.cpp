#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> bombs;

bool cmp_bombs(int a, int b) {
    return bombs[a] > bombs[b];
}

void testcase() {
    int n; cin >> n;
    bombs = vector<int>(n);
    vector<int> order(n);
    vector<bool> defused(n,false);
    for(int i = 0; i < n; i++) {
        cin >> bombs[i];
        order[i] = i;
    }
    sort(order.begin(), order.end(), cmp_bombs);
    int time = 0;
    while(!order.empty()) {
        while(!order.empty() && defused[order.back()]) {
            order.pop_back();
        }
        if(order.empty()) {
            break;
        }
        int next = order.back();
        int left = 2 * next + 1;
        int right = left + 1;
        if(n <= left || (defused[left] && defused[right])) {
            if(time < bombs[next]) {
                order.pop_back();
                defused[next] = true;
                time++;
            }
            else {
                cout << "no" << endl;
                return;
            }
        }
        else {
            order.push_back(left);
            order.push_back(right);
        }
    }
    cout << "yes" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
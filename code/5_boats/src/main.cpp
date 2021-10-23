#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool cmp_boats(const pair<int,int>& a, const pair<int,int>& b) {
    return a.second < b.second;
}

void testcase() {
    int n; cin >> n;
    vector<pair<int,int>> boats(n);
    for(auto& b : boats) {
        cin >> b.first >> b.second;
    }
    sort(boats.begin(), boats.end(), cmp_boats);
    int end = boats[0].second;
    int count = 1;
    int i = 1;
    while(i < n) {
        if(end <= boats[i].second) {
            int current_min = max(end + boats[i].first, boats[i].second);
            i++;
            while(i < n && boats[i].second < current_min) {
                int new_min = max(end + boats[i].first, boats[i].second);
                if(new_min < current_min) {
                    current_min = new_min;
                }
                i++;
            }
            end = current_min;
            count++;
        }
        else {
            i++;
        }
    }
    cout << count << endl;
}

int main() {
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
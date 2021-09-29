#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int DISTANCE = 201;

void count_trigger(const vector<int>& parasols, int count, int start, int end, vector<pair<int,int>>& intervals, int& min_distance, int& max_count) {
    if(max_count < count) {
        max_count = count;
        int span = parasols[end] - parasols[start];
        min_distance = span / 2;
        if(span % 2) {
            min_distance++;
        }
        intervals = { make_pair(start,end) };
    }
    else if(max_count == count) {
        int current_span = (parasols[end]-parasols[start]);
        int current_distance = current_span / 2;
        if(current_span % 2) {
            current_distance++;
        }
        if(current_distance < min_distance) {
            min_distance = current_distance;
            intervals = { make_pair(start,end) };
        }
        else if(current_distance == min_distance) {
            intervals.emplace_back(start,end);
        }
    }
}

void testcase() {
    int n; cin >> n;
    vector<int> parasols(n);
    for(int i = 0; i < n; i++) {
        cin >> parasols[i];
    }
    sort(parasols.begin(),parasols.end());
    int start_pos = parasols[0];
    int min_distance = 0;
    int count = 1;
    int max_count = 1;
    int start = 0;
    int end = 0;
    vector<pair<int,int>> intervals = { make_pair(start,end) };
    for(int i = 1; i < n; i++) {
        count++;
        end = i;
        if(parasols[i] - start_pos < DISTANCE) {
            count_trigger(parasols, count, start, end, intervals, min_distance, max_count);
        }
        else {
            while(DISTANCE <= parasols[i] - start_pos && start < n + 1) {
                start++;
                start_pos = parasols[start];
                count--;
            }
            count_trigger(parasols, count, start, end, intervals, min_distance, max_count);
        }
    }
    vector<int> coordinates;
    for(auto e : intervals) {
        int coordinate = (parasols[e.first] + parasols[e.second]) / 2;
        coordinates.push_back(coordinate);
        if((parasols[e.second] - parasols[e.first]) % 2) {
            int adjustment = coordinate < 0 ? -1 : 1;
            coordinates.push_back(coordinate+adjustment);
        }
    }
    sort(coordinates.begin(),coordinates.end());
    string s;
    s += to_string(max_count) + " " + to_string(min_distance) + "\n";
    for(int c : coordinates) {
        s += to_string(c) + " ";
    }
    cout << s.substr(0,s.size()-1) << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for (int i = 0; i < t; ++i) {
        testcase();
    }
    return 0;
}
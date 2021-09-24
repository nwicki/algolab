#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>

using namespace std;

void testcase() {
    int n,m,v; cin >> n >> m >> v;
    vector<vector<int>> graph(n,vector<int>());
    for(int i = 0; i < m; i++) {
        int a,b; cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    for(auto& nh : graph) {
        sort(nh.begin(),nh.end());
    }
    vector<pair<int,int>> timestamps(n, make_pair(-1,-1));
    stack<int> to_visit;
    vector<bool> visited(n,false);
    int timestamp = 0;
    timestamps[v].first = timestamp++;
    to_visit.push(v);
    while(!to_visit.empty()) {
        int current = to_visit.top();
        visited[current] = true;
        auto neighbours = graph[current];
        auto it = neighbours.begin();
        while(it != neighbours.end() && visited[*it]) {
            it++;
        }
        if(it == neighbours.end()) {
            timestamps[to_visit.top()].second = timestamp++;
            to_visit.pop();
        }
        else {
            timestamps[*it].first = timestamp++;
            to_visit.push(*it);
        }
    }
    string discovery;
    string finishing;
    for(auto df : timestamps) {
        discovery += to_string(df.first) + " ";
        finishing += to_string(df.second) + " ";
    }
    cout << discovery << endl;
    cout << finishing << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 0; i < t; ++i) {
        testcase();
    }
    return 0;
}
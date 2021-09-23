#include <iostream>
#include <vector>
#include <queue>

using namespace std;

void testcase() {
    int n,m,v; cin >> n >> m >> v;
    vector<vector<int>> graph(n,vector<int>());
    for(int i = 0; i < m; i++) {
        int a,b; cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    vector<int> distances(n,-1);
    queue<int> to_visit;
    vector<bool> visited(n,false);
    to_visit.push(v);
    distances[v] = 0;
    while(!to_visit.empty()) {
        int current = to_visit.front();
        to_visit.pop();
        visited[current] = true;
        int new_distance = distances[current] + 1;
        for(int neighbour : graph[current]) {
            if(!visited[neighbour]) {
                to_visit.push(neighbour);
                int distance = distances[neighbour];
                if(distance == -1 || new_distance < distance) {
                    distances[neighbour] = new_distance;
                }
            }
        }
    }
    for(int distance : distances) {
        cout << distance << " ";
    }
    cout << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 0; i < t; ++i) {
        testcase();
    }
    return 0;
}
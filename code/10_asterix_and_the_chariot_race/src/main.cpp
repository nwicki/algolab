#include <iostream>
#include <vector>

using namespace std;

vector<int> parent;
vector<vector<int>> children;
vector<int> cost;
vector<bool> repaired;
vector<bool> covered;

void repair(int index) {
//    cout << "Visiting " << index << endl;
    if(children[index].empty()) {
        if(parent[index] == -1) {
            repaired[index] = true;
//            cout << "Repair " << index << " as parent cost is lower." << endl;
            return;
        }
        if(cost[parent[index]] <= cost[index]) {
            repaired[parent[index]] = true;
//            cout << "Repair " << parent[index] << " as own cost is lower." << endl;
        }
        return;
    }
    for(auto e : children[index]) {
        repair(e);
    }
    bool children_covered = true;
    int children_cost = 0;
    for(auto e : children[index]) {
        if(repaired[e] || repaired[index]) {
            covered[e] = true;
            covered[index] = true;
        }
        children_covered = covered[e] && children_covered;
        if(!covered[e]) {
            children_cost += cost[e];
        }
    }
    if(!children_covered) {
        if(cost[index] <= children_cost) {
            repaired[index] = true;
//            cout << "Repair " << index << " as own cost is lower." << endl;
            covered[index] = true;
            for(auto e : children[index]) covered[e] = true;
        }
        else {
            covered[index] = true;
            for(auto e : children[index]) {
                if(!covered[e]) {
                    covered[e] = true;
                    repaired[e] = true;
//                    cout << "Repair " << e << " as children cost is lower." << endl;
                }
            }
        }
    }
    if(!covered[index]) {
        int min = 1 << 30;
        int min_index = 0;
//        cout << "Step through children of " << index << endl;
        for(auto e : children[index]) {
//            cout << "Check child " << e << endl;
            if(cost[e] < min) {
                min = cost[e];
                min_index = e;
            }
        }
//        cout << "Minimal child is " << min_index << endl;
        if(cost[min_index] < cost[index]) {
            if(parent[index] != -1) {
                if(cost[min_index] < cost[parent[index]]) {
//                    cout << "Repair " << min_index << " as minimal child cost is lower." << endl;
                    repaired[min_index] = true;
                    covered[index] = true;
                }
                else {
//                    cout << "Repair " << parent[index] << " as parent cost is lower." << endl;
                    repaired[parent[index]] = true;
                    covered[index] = true;
                }
            }
            else {
                repaired[min_index] = true;
                covered[index] = true;
            }
        }
        if(parent[index] == -1) {
            repaired[index] = true;
            covered[index] = true;
        }
    }
}

void testcase() {
    int n; cin >> n;
    parent = vector<int>(n, -1);
    children = vector<vector<int>>(n, vector<int>());
    for(int i = 0; i < n-1; i++) {
        int u,v; cin >> u >> v;
        children[u].push_back(v);
        parent[v] = u;
    }
    cost = vector<int>(n);
    for(auto& c : cost) {
        cin >> c;
    }
    repaired = vector<bool>(n, false);
    covered = vector<bool>(n, false);
    repair(0);
    int repair_cost = 0;
    for(int i = 0; i < n; i++) {
        if(repaired[i]) repair_cost += cost[i];
    }
    cout << repair_cost << endl;
}


int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
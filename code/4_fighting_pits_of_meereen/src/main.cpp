#include <iostream>
#include <vector>
#include <cmath>
#include <queue>

using namespace std;

const int EXCITE = 1000;

class gate {
public:
    int types;
    int total;
    vector<int> distinct;
    int capacity;
    queue<int> memory;
    explicit gate(int k, int m) {
        types = 0;
        total = 0;
        capacity = m;
        distinct = vector<int>(k,0);
        memory = queue<int>();
    }
    void add_fighter(int type) {
        if(memory.size() == capacity) {
            remove_fighter();
        }
        int amount = distinct[type];
        types += amount == 0;
        amount++;
        distinct[type] = amount;
        memory.push(type);
        total++;
    }
    void remove_fighter() {
        int amount = distinct[memory.front()];
        amount--;
        types -= amount == 0;
        distinct[memory.front()] = amount;
        memory.pop();
    }
};

int excitement(const gate& chosen_gate, const gate& other_gate, int type) {
    return chosen_gate.types * EXCITE - (1 << abs(chosen_gate.total - other_gate.total));
}

int send_fighters_0(const vector<int>& fighters, gate& south, gate& north, int f) {
    int fighter = fighters[f];
    gate south_next = south;
    gate north_next = north;
    south_next.add_fighter(fighter);
    north_next.add_fighter(fighter);
    if(f == fighters.size() - 1) {
        return max(excitement(north_next, south, fighter), excitement(south_next, north, fighter));
    }
    return max(excitement(north_next, south, fighter) + send_fighters_0(fighters, south, north_next, f + 1),
               excitement(south_next, north, fighter) + send_fighters_0(fighters, south_next, north, f + 1));
}

void compute_0(const vector<int>& fighters, int k, int m) {
    gate south(k,m);
    gate north(k,m);
    cout << send_fighters_0(fighters, south, north, 0) << endl;
}

int send_fighters_20(const vector<int>& fighters, gate& south, gate& north, int f) {
    int fighter = fighters[f];
    gate south_next = south;
    gate north_next = north;
    south_next.add_fighter(fighter);
    north_next.add_fighter(fighter);
    if(f == fighters.size() - 1) {
        return max(excitement(north_next, south, fighter), excitement(south_next, north, fighter));
    }
    return max(excitement(north_next, south, fighter) + send_fighters_0(fighters, south, north_next, f + 1),
               excitement(south_next, north, fighter) + send_fighters_0(fighters, south_next, north, f + 1));
}

void compute_20(const vector<int>& fighters, int k, int m) {
    gate south(k,m);
    gate north(k,m);
    cout << send_fighters_20(fighters, south, north, 0) << endl;
}

void testcase() {
    int n,k,m; cin >> n >> k >> m;
    vector<int> fighters = vector<int>(n);
    for(int& f : fighters) {
        cin >> f;
    }
    compute_20(fighters,k,m);
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
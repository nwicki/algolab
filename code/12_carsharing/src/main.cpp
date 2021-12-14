#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, long,
        property<edge_residual_capacity_t, long,
            property<edge_reverse_t, traits::edge_descriptor,
                property<edge_weight_t, long>>>>> graph_t;
typedef graph_traits<graph_t>::edge_descriptor edge_t;
typedef graph_traits<graph_t>::vertex_descriptor vertex_t;

void add_edge(graph_t& g, vertex_t u, vertex_t v, long capacity, long cost) {
    auto c_map = get(edge_capacity, g);
    auto r_map = get(edge_reverse, g);
    auto w_map = get(edge_weight, g);
    auto e = add_edge(u, v, g).first;
    auto rev_e = add_edge(v, u, g).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
}

class route {
public:
    size_t u;
    size_t v;
    size_t d;
    size_t a;
    long p;
};

const bool PRINT = false;

void testcase() {
    size_t n, s; cin >> n >> s;
    vector<long> cars(s);
    for(auto& c : cars) {
        cin >> c;
    }
    vector<route> routes(n);
    vector<map<size_t,vertex_t>> indices(s);
    vector<vector<pair<size_t,vertex_t>>> indices_sorted(s);
    size_t vertices = 0;
    for(auto& r : routes) {
        cin >> r.u >> r.v >> r.d >> r.a >> r.p;
        r.u--; r.v--;
        if(indices[r.u].find(r.d) == indices[r.u].end()) {
            indices[r.u].insert(make_pair(r.d,vertices));
            indices_sorted[r.u].emplace_back(r.d, vertices);
            vertices++;
        }
        if(indices[r.v].find(r.a) == indices[r.v].end()) {
            indices[r.v].insert(make_pair(r.a,vertices));
            indices_sorted[r.v].emplace_back(r.a, vertices);
            vertices++;
        }
    }
    graph_t g(vertices);
    const long adjust = 0;
    const long MAX = 1L << 62;
    vertex_t source = add_vertex(g);
    vertex_t target = add_vertex(g);
    for(size_t i = 0; i < s; i++) {
        sort(indices_sorted[i].begin(), indices_sorted[i].end());
        auto ind = indices_sorted[i];
        add_edge(g, source, ind.front().second, cars[i], 0);
        if(PRINT) cout << "Station " << i << ": ";
        for(auto& e : ind) {
            if(PRINT) cout << "(" << e.first << "," << e.second << "), ";
        }
        if(PRINT) cout << endl;
        if(PRINT) cout << "Source " << source << " to station " << i << ": (" << ind.begin()->first << "," << ind.begin()->second << ")" << endl;
        add_edge(g, ind.back().second, target, MAX, 0);
        if(PRINT) cout << "Station " << i << " to target " << target << ": (" << ind.back().first << "," << ind.back().second << ")" << endl;
    }
    for(auto& ind : indices_sorted) {
        for(size_t j = 0; j < ind.size()-1; j++) {
            add_edge(g, ind[j].second, ind[j+1].second, MAX, adjust);
            if(PRINT) cout << "Station connection: (" << ind[j].first << "," << ind[j].second << ") to (" << ind[j+1].first << "," << ind[j+1].second << ")" << endl;
        }
    }
    for(auto& r : routes) {
        size_t start = indices[r.u].find(r.d)->second;
        size_t end = indices[r.v].find(r.a)->second;
        add_edge(g, start, end, 1, -r.p + adjust);
        if(PRINT) cout << "Route: (" << indices[r.u].find(r.d)->first << "," << indices[r.u].find(r.d)->second << ") to (" << indices[r.v].find(r.a)->first << "," << indices[r.v].find(r.a)->second << ") with " << 1 << "," << -r.p + adjust << endl;
    }
    push_relabel_max_flow(g, source, target);
    cycle_canceling(g);
//    successive_shortest_path_nonnegative_weights(g, source, target);
    cout << -find_flow_cost(g) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
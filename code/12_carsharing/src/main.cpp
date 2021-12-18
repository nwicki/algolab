#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
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

edge_t add_edge(graph_t& g, vertex_t u, vertex_t v, long capacity, long cost) {
    auto w_map = get(edge_weight, g);
    auto r_map = get(edge_reverse, g);
    auto c_map = get(edge_capacity, g);
    auto e = add_edge(u, v, g).first;
    auto rev_e = add_edge(v, u, g).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
    return e;
}

class route {
public:
    long u; long v;
    long d; long a;
    long p;
};

void testcase() {
    size_t n, s; cin >> n >> s;
    vector<long> cars(s);
    for(auto& c : cars) {
        cin >> c;
    }
    vector<route> routes(n);
    vector<map<long,vertex_t>> indices(s);
    size_t num_vertices = 0;
    long last_arrival = 0;
    for(auto& r : routes) {
        cin >> r.u >> r.v >> r.d >> r.a >> r.p; r.u--; r.v--;
        last_arrival = max(last_arrival, r.a);
        if(indices[r.u].find(r.d) == indices[r.u].end()) {
            indices[r.u][r.d] = num_vertices;
            num_vertices++;
        }
        if(indices[r.v].find(r.a) == indices[r.v].end()) {
            indices[r.v][r.a] = num_vertices;
            num_vertices++;
        }
    }
    graph_t g(num_vertices);
    vertex_t source = add_vertex(g);
    vertex_t target = add_vertex(g);
    map<edge_t,long> forward;
    const long adjust = 100;
    const long MAX = 1L << 62;
    for(size_t i = 0; i < s; i++) {
        if(indices[i].empty()) continue;
        auto indi = vector<pair<long,vertex_t>>(indices[i].begin(),indices[i].end());
        sort(indi.begin(), indi.end());
        long cost_start = adjust * indi.front().first;
        forward[add_edge(g, source, indi.front().second, cars[i], cost_start)] = cost_start;
        for(size_t j = 0; j < indi.size()-1; j++) {
            long cost_internal = adjust * (indi[j+1].first - indi[j].first);
            forward[add_edge(g, indi[j].second, indi[j+1].second, MAX, cost_internal)] = cost_internal;
        }
        long cost_end = adjust * (last_arrival - indi.back().first);
        forward[add_edge(g, indi.back().second, target, MAX, cost_end)] = cost_end;
    }
    for(const auto& r : routes) {
        long cost_route = adjust * (r.a - r.d);
        forward[add_edge(g, indices[r.u][r.d], indices[r.v][r.a], 1, -r.p + cost_route)] = cost_route;
   }
    successive_shortest_path_nonnegative_weights(g, source, target);
    auto c_map = get(edge_capacity, g);
    auto r_map = get(edge_residual_capacity, g);
    long total = 0;
    for(auto e : forward) {
        long diff = c_map[e.first] - r_map[e.first];
        if(diff) {
            total += e.second * diff;
        }
    }
    cout << total - find_flow_cost(g) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
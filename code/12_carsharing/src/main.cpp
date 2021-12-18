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
    long u;
    long v;
    long d;
    long a;
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
    vector<vector<pair<long,vertex_t>>> indices_sorted(s);
    map<edge_t,long> forward;
    size_t num_vertices = 0;
    long last_arrival = 0;
    for(auto& r : routes) {
        cin >> r.u >> r.v >> r.d >> r.a >> r.p;
        r.u--; r.v--;
        last_arrival = max(last_arrival, r.a);
        if(indices[r.u].find(r.d) == indices[r.u].end()) {
            indices[r.u].insert(make_pair(r.d,num_vertices));
            indices_sorted[r.u].emplace_back(r.d, num_vertices);
            num_vertices++;
        }
        if(indices[r.v].find(r.a) == indices[r.v].end()) {
            indices[r.v].insert(make_pair(r.a,num_vertices));
            indices_sorted[r.v].emplace_back(r.a, num_vertices);
            num_vertices++;
        }
    }
    graph_t g(num_vertices);
    const long adjust = 100;
    const long MAX = 1L << 62;
    vertex_t source = add_vertex(g);
    vertex_t target = add_vertex(g);
    for(size_t i = 0; i < s; i++) {
        auto& indi = indices_sorted[i];
        sort(indi.begin(), indi.end());
        auto u = indi.front();
        auto e = add_edge(g, source, u.second, cars[i], adjust * u.first);
        forward[e] = adjust * u.first;
        auto v = indi.back();
        e = add_edge(g, v.second, target, MAX, adjust * (last_arrival - v.first));
        forward[e] = adjust * (last_arrival - v.first);
    }
    for(auto& ind : indices_sorted) {
        for(size_t j = 0; j < ind.size()-1; j++) {
            auto u = ind[j];
            auto v = ind[j+1];
            auto e = add_edge(g, u.second, v.second, MAX, adjust * (v.first - u.first));
            forward[e] = adjust * (v.first - u.first);
        }
    }
    for(auto& r : routes) {
        auto u = *indices[r.u].find(r.d);
        auto v = *indices[r.v].find(r.a);
        auto e = add_edge(g, u.second, v.second, 1, -r.p + adjust * (v.first - u.first));
        forward[e] = adjust * (v.first - u.first);
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
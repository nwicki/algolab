#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <vector>
#include <queue>

using namespace boost;
using namespace std;

typedef adjacency_list_traits<vecS, vecS, directedS> traits;
typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_capacity_t, long,
            property<edge_residual_capacity_t, long, property<edge_reverse_t, traits::edge_descriptor,
                property <edge_weight_t, long>>>>> graph_t;
typedef graph_traits<graph_t>::edge_descriptor edge_t;
typedef graph_traits<graph_t>::vertex_descriptor vertex_t;

void add_edge(graph_t& g, vertex_t u, vertex_t v, long capacity, long cost) {
    auto c_map = get(edge_capacity, g);
    auto r_map = get(edge_reverse, g);
    auto w_map = get(edge_weight, g);
    const edge_t e = add_edge(u, v, g).first;
    const edge_t rev_e = add_edge(v, u, g).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
}

const long ADJUST = 20;
void testcase() {
    size_t n; cin >> n;
    graph_t days(n);
    vertex_t v_source = add_vertex(days);
    vertex_t v_target = add_vertex(days);
    long students = 0;
    for(vertex_t i = 0; i < n; i++) {
        long a, c; cin >> a >> c;
        add_edge(days, v_source, i, a, c);
    }
    for(vertex_t i = 0; i < n; i++) {
        long s, p; cin >> s >> p;
        students += s;
        add_edge(days, i, v_target, s, -p + ADJUST);
    }
    for(vertex_t i = 0; i < n-1; i++) {
        long v, e; cin >> v >> e;
        add_edge(days, i, i+1, v, e);
    }
    successive_shortest_path_nonnegative_weights(days, v_source, v_target);
    long flow = 0;
    auto c_map = get(edge_capacity, days);
    auto rc_map = get(edge_residual_capacity, days);
    for(auto e = out_edges(v_source, days); e.first != e.second; e.first++) {
        flow += c_map[*e.first] - rc_map[*e.first];
    }
    if(flow != students) {
        cout << "im";
    }
    cout << "possible " << flow << " " << (flow * ADJUST - find_flow_cost(days)) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
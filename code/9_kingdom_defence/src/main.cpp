#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/graph_traits.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> traits;
typedef adjacency_list<vecS, vecS, directedS, no_property, property<
                        edge_capacity_t, long, property<
                                edge_residual_capacity_t, long, property<
                                        edge_reverse_t, traits::edge_descriptor>>>> graph_t;
typedef traits::vertex_descriptor vertex_t;

void add_edge(graph_t& g, vertex_t u, vertex_t v, long c) {
    auto c_map = get(edge_capacity, g);
    auto r_map = get(edge_reverse, g);
    auto e = add_edge(u , v, g).first;
    auto rev_e = add_edge(v , u, g).first;
    c_map[e] = c;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
}

void testcase() {
    size_t l, p; cin >> l >> p;
    graph_t g(l);
    vertex_t source = add_vertex(g);
    vertex_t target = add_vertex(g);
    for(size_t i = 0; i < l; i++) {
        long gi, di; cin >> gi >> di;
        add_edge(g, source, i, gi);
        add_edge(g, i, target, di);
    }
    auto c_map = get(edge_capacity, g);
    for(size_t i = 0; i < p; i++) {
        vertex_t u,v; cin >> u >> v;
        long cj, Cj; cin >> cj >> Cj;
        add_edge(g, u, v, Cj - cj);
        c_map[edge(source, v, g).first] += cj;
        c_map[edge(u, target, g).first] += cj;
    }
    long goal = 0;
    for(size_t i = 0; i < l; i++) {
        goal += c_map[edge(i, target, g).first];
    }
    push_relabel_max_flow(g, source, target) == goal ? cout << "yes" << endl : cout << "no" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
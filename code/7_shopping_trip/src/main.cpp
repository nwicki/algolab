#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> traits;
typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_capacity_t, long, property<
                        edge_residual_capacity_t, long, property<
                            edge_reverse_t, traits::edge_descriptor>>>> graph_t;
typedef traits::vertex_descriptor vertex_t;

void add_edge(graph_t& g, vertex_t u, vertex_t v, long c) {
    auto c_map = get(edge_capacity, g);
    auto r_map = get(edge_reverse, g);
    auto e = add_edge(u, v, g).first;
    auto rev_e = add_edge(v, u, g).first;
    c_map[e] = c;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
}

void testcase() {
    size_t n, m, s; cin >> n >> m >> s;
    graph_t g(n + 2 * m);
    auto target = add_vertex(g);
    for(size_t i = 0; i < s; i++) {
        vertex_t si; cin >> si;
        add_edge(g, si, target, 1L);
    }
    for(size_t i = 0; i < m; i++) {
        vertex_t u,v; cin >> u >> v;
        vertex_t in = n + 2 * u;
        vertex_t out = in + 1;
        add_edge(g, in, out, 1L);
        add_edge(g, u, in, 1L);
        add_edge(g, out, v, 1L);
        add_edge(g, v, in, 1L);
        add_edge(g, out, u, 1L);
    }
    if((size_t) push_relabel_max_flow(g, *vertices(g).first, target) == s) {
        cout << "yes" << endl;
        return;
    }
    cout << "no" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> traits;
typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_capacity_t, long,
                        property<edge_residual_capacity_t, long,
                            property<edge_reverse_t, traits::edge_descriptor>
                        >
                      >> graph_t;

typedef traits::vertex_descriptor vertex_t;
typedef traits::edge_descriptor edge_t;

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
    int n; int m; cin >> n >> m;
    graph_t g(n+m);
    vertex_t source = add_vertex(g);
    for(int i = 0; i < m; i++) {
        int a,b,c; cin >> a >> b >> c;
        int game = n+i;
        add_edge(g, source, game, 1);
        if(c) {
            add_edge(g, game, c == 1 ? a : b, 1);
        }
        else {
            add_edge(g, game, a, 1);
            add_edge(g, game, b, 1);
        }
    }
    vertex_t target = add_vertex(g);
    long total = 0;
    for(int i = 0; i < n; i++) {
        int s; cin >> s;
        add_edge(g, i, target, s);
        total += s;
    }
    long flow = push_relabel_max_flow(g, source, target);
    if(flow != m || total != m) {
        cout << "no" << endl;
        return;
    }
    cout << "yes" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
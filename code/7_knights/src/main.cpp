#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/graph_traits.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
                        property<edge_capacity_t, long,
                            property<edge_residual_capacity_t, long,
                                property<edge_reverse_t, traits::edge_descriptor>>>> graph_t;
typedef traits::edge_descriptor edge_t;
typedef traits::vertex_descriptor vertex_t;

void add_edge(vertex_t u, vertex_t v, long c, graph_t& g) {
    auto c_map = get(edge_capacity, g);
    auto r_map = get(edge_reverse, g);
    auto e = add_edge(u, v, g).first;
    auto rev_e = add_edge(v, u, g).first;
    c_map[e] = c;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
}

int in_index(int row, int col, int m) {
    return 2 * (row * m + col);
}

int out_index(int row, int col, int m) {
    return in_index(row, col, m) + 1;
}

void testcase() {
    int m,n,k; long c; cin >> m >> n >> k >> c;
    graph_t cave(2 * m * n);
    auto source = add_vertex(cave);
    auto target = add_vertex(cave);
    for(int i = 0; i < k; i++) {
        int col, row; cin >> col >> row;
        add_edge(source, in_index(row, col, m), 1L, cave);
    }
    if(!k || m <= 1 || n <= 1) {
        cout << k << endl;
        return;
    }
    // Intersections
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            add_edge(in_index(i, j, m), out_index(i, j, m), c, cave);
        }
    }
    // Hallways
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            int index = out_index(i, j, m);
            if(i != 0) {
                add_edge(index, in_index(i - 1, j, m), 1L, cave);
            }
            else {
                add_edge(index, target, 1L, cave);
            }
            if(i != n - 1) {
                add_edge(index, in_index(i + 1, j, m), 1L, cave);
            }
            else {
                add_edge(index, target, 1L, cave);
            }
            if(j != 0) {
                add_edge(index, in_index(i, j - 1, m), 1L, cave);
            }
            else {
                add_edge(index, target, 1L, cave);
            }
            if(j != m - 1) {
                add_edge(index, in_index(i, j + 1, m), 1L, cave);
            }
            else {
                add_edge(index, target, 1L, cave);
            }
        }
    }
    cout << push_relabel_max_flow(cave, source, target) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <vector>
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
                property <edge_weight_t, long>>>>> graph_t;

typedef graph_traits<graph_t>::edge_descriptor edge_t;
typedef graph_traits<graph_t>::vertex_descriptor vertex_t;

int index(int r, int c, int n) {
    return r * n + c;
}

void add_edge(graph_t& G, vertex_t from, vertex_t to, long capacity, long cost) {
    auto c_map = get(edge_capacity, G);
    auto r_map = get(edge_reverse, G);
    auto w_map = get(edge_weight, G);
    const auto e = add_edge(from, to, G).first;
    const auto rev_e = add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
}

void testcase() {
    int n; cin >> n;
    vector<vector<int>> matrix(n, vector<int>(n));
    for(auto& row : matrix) {
        for(auto& col : row) {
            cin >> col;
        }
    }
    long adjust = 100;
    graph_t g(n + n - 1 + 2 * (n-1) * (n-1)-1);
    for(int i = 1; i < n; i++) {
        add_edge(g, index(0,i-1,n), index(0,i,n), 1, -matrix[0][i] + adjust);
        add_edge(g, index(i-1,0,n), index(i,0,n), 1, -matrix[i][0] + adjust);
    }
    vertex_t restrictor = n * n;
    for(int i = 1; i < n; i++) {
        for(int j = 1; j < n; j++) {
            if(i == n-1 && j == n-1) {
                add_edge(g, index(i-1,j,n), index(i,j,n), 1, 0);
                add_edge(g, index(i,j-1,n), index(i,j,n), 1, 0);
            }
            else {
                add_edge(g, index(i-1,j,n), restrictor, 1, 0);
                add_edge(g, index(i,j-1,n), restrictor, 1, 0);
                add_edge(g, restrictor, index(i,j,n), 1, -matrix[i][j] + adjust);
                restrictor++;
            }
        }
    }
    successive_shortest_path_nonnegative_weights(g, 0, n*n-1);
    cout << ((4 * n - 6) * adjust - find_flow_cost(g) + matrix[0][0] + matrix[n-1][n-1]) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
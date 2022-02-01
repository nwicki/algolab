#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long>>>>> graph;

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

class edge_adder {
    graph &G;

    public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(vertex_desc from, vertex_desc to, long capacity, long cost) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0;
        r_map[e] = rev_e;
        r_map[rev_e] = e;
        w_map[e] = cost;
        w_map[rev_e] = -cost;
    }
};

void testcase() {
    size_t e, w, m, d; long p, l; std::cin >> e >> w >> m >> d >> p >> l;
    graph G(2 * (e + w));
    edge_adder adder(G);
    vertex_desc v_source = boost::add_vertex(G);
    vertex_desc v_e_matches = boost::add_vertex(G);
    long e_matches = p - l * e;
    adder.add_edge(v_source, v_e_matches, e_matches, 0);
    for(vertex_desc i = 0; i < e; i++) {
        adder.add_edge(v_source, 2 * i, l, 0);
        adder.add_edge(v_e_matches, 2 * i, e_matches, 0);
        adder.add_edge(v_e_matches, 2 * i + 1, e_matches, 0);
    }
    vertex_desc v_target = boost::add_vertex(G);
    vertex_desc v_w_matches = boost::add_vertex(G);
    long w_matches = p - l * w;
    adder.add_edge(v_w_matches, v_target, w_matches, 0);
    for(vertex_desc i = e; i < e + w; i++) {
        adder.add_edge(2 * i, v_target, l, 0);
        adder.add_edge(2 * i, v_w_matches, w_matches, 0);
        adder.add_edge(2 * i + 1, v_w_matches, w_matches, 0);
    }
    for(vertex_desc i = 0; i < m; i++) {
        vertex_desc u, v; long r; std::cin >> u >> v >> r;
        adder.add_edge(2 * u, 2 * (e + v), 1, r);
    }
    for(vertex_desc i = 0; i < d; i++) {
        vertex_desc u, v; long r; std::cin >> u >> v >> r;
        adder.add_edge(2 * u + 1, 2 * (e + v) + 1, 1, r);
    }
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    long flow = 0;
    auto out = boost::out_edges(v_source, G);
    for(auto it = out.first; it != out.second; it++) flow += c_map[*it] - rc_map[*it];
    std::cout << (flow == p ? std::to_string(boost::find_flow_cost(G)) : "No schedule!") << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();

}
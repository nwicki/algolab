#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::vertex_descriptor            vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

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
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

const long ADJUST = 100;

void testcase() {
    vertex_desc n, m, s; std::cin >> n >> m >> s;
    graph g(n + m + s);
    edge_adder adder(g);
    auto v_source = boost::add_vertex(g);
    auto v_target = boost::add_vertex(g);
    for(vertex_desc i = 0; i < n; i++) {
        adder.add_edge(v_source, i, 1, 0);
    }
    for(vertex_desc i = 0; i < s; i++) {
        long l; std::cin >> l;
        adder.add_edge(n + m + i, v_target, l, 0);
    }
    for(size_t i = 0; i < m; i++) {
        vertex_desc st; std::cin >> st;
        st--;
        adder.add_edge(n + i, n + m + st, 1, 0);
    }
    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < m; j++) {
            long bid; std::cin >> bid;
            adder.add_edge(i, n + j, 1L, ADJUST - bid);
        }
    }
    boost::successive_shortest_path_nonnegative_weights(g, v_source, v_target);
    long cost = boost::find_flow_cost(g);
    long flow = 0;
    auto c_map = boost::get(boost::edge_capacity, g);
    auto rc_map = boost::get(boost::edge_residual_capacity, g);
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,g), g); e != eend; ++e) {
        flow += c_map[*e] - rc_map[*e];
    }
    std::cout << flow << " " << (flow * ADJUST - cost) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
}
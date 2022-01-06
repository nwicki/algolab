#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(vertex_desc from, vertex_desc to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

const long MAX = 1L << 62;

void testcase() {
    long n; std::cin >> n;
    std::vector<std::vector<long>> index(n, std::vector<long>(n, -1));
    long n_vertices = 0;
    for(long i = 0; i < n; i++) {
        for(long j = 0; j < n; j++) {
            bool e; std::cin >> e;
            if(e) index[i][j] = n_vertices++;
        }
    }
    graph g(n_vertices);
    edge_adder adder(g);
    vertex_desc v_source = boost::add_vertex(g);
    vertex_desc v_target = boost::add_vertex(g);
    for(long i = 0; i < n; i++) {
        for(long j = 0; j < n; j++) {
            long out = index[i][j];
            if(out == -1) continue;
            if((i + j) & 1L) {
                adder.add_edge(v_source, out, 1L);
                if(0 < i &&   1 < j &&   index[i-1][j-2] != -1) adder.add_edge(out, index[i-1][j-2], 1L);
                if(0 < i &&   j < n-2 && index[i-1][j+2] != -1) adder.add_edge(out, index[i-1][j+2], 1L);
                if(i < n-1 && 1 < j &&   index[i+1][j-2] != -1) adder.add_edge(out, index[i+1][j-2], 1L);
                if(i < n-1 && j < n-2 && index[i+1][j+2] != -1) adder.add_edge(out, index[i+1][j+2], 1L);
                if(1 < i &&   0 < j &&   index[i-2][j-1] != -1) adder.add_edge(out, index[i-2][j-1], 1L);
                if(i < n-2 && 0 < j &&   index[i+2][j-1] != -1) adder.add_edge(out, index[i+2][j-1], 1L);
                if(1 < i &&   j < n-1 && index[i-2][j+1] != -1) adder.add_edge(out, index[i-2][j+1], 1L);
                if(i < n-2 && j < n-1 && index[i+2][j+1] != -1) adder.add_edge(out, index[i+2][j+1], 1L);
            }
            else adder.add_edge(out, v_target, 1L);
        }
    }
    std::cout << n_vertices - boost::push_relabel_max_flow(g, v_source, v_target) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
}

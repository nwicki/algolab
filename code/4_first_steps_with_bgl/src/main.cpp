#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <vector>

using namespace std;

typedef boost::adjacency_list<
            boost::vecS,
            boost::vecS,
            boost::undirectedS,
            boost::no_property,
            boost::property<boost::edge_weight_t, int>
            > weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;


void testcase() {
    int n,m; cin >> n >> m;
    weighted_graph G(n);
    weight_map map = boost::get(boost::edge_weight, G);
    for(int i = 0; i < m; i++) {
        int e1,e2,w; cin >> e1 >> e2 >> w;
        edge_desc e = boost::add_edge(e1,e2,G).first;
        map[e] = w;
    }
    vector<edge_desc> mst;
    vector<int> dist_map(n);

    boost::kruskal_minimum_spanning_tree(G, back_inserter(mst));
    int sum = 0;
    for(auto e : mst) {
        sum += map[e];
    }
    boost::dijkstra_shortest_paths(G, 0, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));
    cout << sum << " " << *max_element(dist_map.begin(),dist_map.end()) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
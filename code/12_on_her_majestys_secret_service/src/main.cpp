#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

void testcase() {
    int n, m, s, c, d; size_t a; std::cin >> n >> m >> a >> s >> c >> d;
    weighted_graph g(n);
    auto w_map = boost::get(boost::edge_weight, g);
    for(int i = 0; i < m; i++) {
        std::string w; int x, y, z; std::cin >> w >> x >> y >> z;
        w_map[boost::add_edge(x, y, g).first] = z;
        if(w == "L") w_map[boost::add_edge(y, x, g).first] = z;
    }
    std::vector<int> agents(a), shelters(s);
    for(auto& e : agents) std::cin >> e;
    for(auto& e : shelters) std::cin >> e;
    int min_dist = d, max_dist = c * d;
    std::vector<std::vector<int>> dist_map(a, std::vector<int>(s, -1));
    int adjust = c * d;
    for(int i = 0; i < a; i++) {
        std::vector<int> temp(n);
        boost::dijkstra_shortest_paths(g, agents[i], boost::distance_map(boost::make_iterator_property_map(temp.begin(), boost::get(boost::vertex_index, g))));
        for(int j = 0; j < s; j++) {
            int dist = temp[shelters[j]];
            if(dist != std::numeric_limits<int>::max()) {
                dist_map[i][j] = dist;
                max_dist = std::max(max_dist, dist + adjust);
            }
        }
    }
    size_t num_shelters = c * s;
    while(min_dist < max_dist) {
        weighted_graph edmond(a + num_shelters);
        int mid_dist = (min_dist + max_dist) / 2;
        for (int i = 0; i < a; i++) {
            for (int j = 0; j < s; j++) {
                int dist = dist_map[i][j];
                if (dist == -1) continue;
                for (int k = 0; k < c; k++)
                    if (dist <= mid_dist - (k + 1) * d) boost::add_edge(i, a + c * j + k, edmond);
            }
        }
        std::vector<vertex_desc> mate_map(boost::num_vertices(edmond));
        boost::edmonds_maximum_cardinality_matching(edmond, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, edmond)));
        if(boost::matching_size(edmond, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, edmond))) == a)
            max_dist = mid_dist;
        else min_dist = mid_dist + 1;
    }
    std::cout << min_dist << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
}
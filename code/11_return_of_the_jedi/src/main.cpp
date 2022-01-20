#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
                                boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

void max_bfs(const weighted_graph& mst_graph, vertex_desc start, std::vector<int>& max_edges) {
    auto weights = boost::get(boost::edge_weight, mst_graph);
    std::vector<bool> visited(boost::num_vertices(mst_graph), false);
    std::queue<std::pair<int, vertex_desc>> to_visit;
    to_visit.emplace(0, start);
    while(!to_visit.empty()) {
        auto top = to_visit.front();
        to_visit.pop();
        visited[top.second] = true;
        max_edges[top.second] = top.first;
        auto e = boost::out_edges(top.second, mst_graph);
        for(auto it = e.first; it != e.second; it++)
            if(!visited[it->m_target]) to_visit.emplace(std::max(top.first, weights[*it]), it->m_target);
    }
}

void testcase() {
    size_t n, temp; std::cin >> n >> temp;
    weighted_graph g(n);
    weight_map weights = boost::get(boost::edge_weight, g);
    for(size_t j = 0; j < n - 1; j++)
        for(size_t k = j + 1; k < n; k++) std::cin >> weights[boost::add_edge(j, k, g).first];
    std::vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(g, std::back_inserter(mst));
    weighted_graph mst_graph(n);
    weight_map mst_weights = boost::get(boost::edge_weight, mst_graph);
    int min_sum = 0;
    for(auto m : mst) {
        int w = weights[m];
        boost::remove_edge(m, g);
        mst_weights[boost::add_edge(m.m_source, m.m_target, mst_graph).first] = w;
        min_sum += w;
    }
    std::vector<std::vector<int>> max_edges(n, std::vector<int>(n));
    for(size_t i = 0; i < n; i++) max_bfs(mst_graph, i, max_edges[i]);
    int min_edge = 1 << 30;
    auto e = boost::edges(g);
    for(auto it = e.first; it != e.second; it++) min_edge = std::min(min_edge, weights[*it] - max_edges[it->m_source][it->m_target]);
    std::cout << min_sum + min_edge << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
}
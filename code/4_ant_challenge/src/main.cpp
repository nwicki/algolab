#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <vector>
#include <queue>

using namespace std;
using namespace boost;

// Undirected graph with simple weights per edge
typedef adjacency_list<
            vecS,
            vecS,
            undirectedS,
            no_property,
            property<edge_weight_t, int>
            > graph_w_u;
typedef property_map<graph_w_u, edge_weight_t>::type map_w_u;
typedef graph_traits<graph_w_u>::edge_descriptor edge_w_u;
typedef graph_traits<graph_w_u>::vertex_descriptor vertex_w_u;

// Undirected graph with a list of weights per edge for each species
typedef adjacency_list<
            vecS,
            vecS,
            undirectedS,
            no_property,
            property<edge_weight_t, vector<int>>
            > graph_wl_u;
typedef graph_traits<graph_wl_u>::edge_descriptor edge_wl_u;
typedef property_map<graph_wl_u, edge_weight_t >::type map_wl_u;
typedef graph_traits<graph_wl_u>::vertex_descriptor vertex_wl_u;

typedef pair<int,pair<vertex_w_u, vertex_w_u>> heap_w_e;

const int MAX = (1L << 31) - 1;

class cmp_edges {
public:
    bool operator() (heap_w_e a, heap_w_e b) {
        return a.first > b.first;
    }
};


void construct_network(const vector<vertex_wl_u>& hives, const graph_wl_u& forest, const map_wl_u& weight_map_forest, graph_w_u& network) {
    map_w_u weight_map_network = get(edge_weight, network);
    for(size_t i = 0; i < hives.size(); i++) {
        vertex_wl_u h = hives[i];
        size_t explored = 1;
        vector<bool> visited(forest.m_vertices.size(),false);
        priority_queue<heap_w_e, vector<heap_w_e>, cmp_edges> heap;
        visited[h] = true;
        auto adj_edges = out_edges(h,forest);
        for(auto it = adj_edges.first; it != adj_edges.second; it++) {
            heap.push(make_pair(weight_map_forest[*it][i],make_pair(it->m_source,it->m_target)));
        }
        while(explored != visited.size()) {
            while(!heap.empty() && visited[heap.top().second.second]) heap.pop();
            auto next_edge = heap.top();
            visited[next_edge.second.second] = true;
            explored++;
            heap.pop();
            auto connections = out_edges(next_edge.second.first,network);
            bool not_found = true;
            for(auto it = connections.first; it != connections.second && not_found; it++) {
                if(it->m_target == next_edge.second.second) {
                    not_found = false;
                    int w = weight_map_network[*it];
                    if(next_edge.first < w) {
                        weight_map_network[*it] = next_edge.first;
                    }
                }
            }
            if(not_found) {
                auto e = add_edge(next_edge.second.first, next_edge.second.second, network).first;
                weight_map_network[e] = next_edge.first;
            }
            adj_edges = out_edges(next_edge.second.second,forest);
            for(auto it = adj_edges.first; it != adj_edges.second; it++) {
                if(!visited[it->m_target]) {
                    heap.push(make_pair(weight_map_forest[*it][i],make_pair(it->m_source,it->m_target)));
                }
            }
        }
    }
}

void testcase() {
    int n,e,s,a,b; cin >> n >> e >> s >> a >> b;
    graph_wl_u forest(n);
    map_wl_u weight_map_forest = get(edge_weight, forest);
    for(int i = 0; i < e; i++) {
        int e1, e2; cin >> e1 >> e2;
        edge_wl_u e3 = add_edge(e1, e2, forest).first;
        vector<int> weights(s);
        for(int& w : weights) {
            cin >> w;
        }
        weight_map_forest[e3] = weights;
    }
    vector<vertex_wl_u> hives(s);
    for(vertex_wl_u& h : hives) {
        cin >> h;
    }
    graph_w_u network(n);
    construct_network(hives,forest,weight_map_forest,network);
    vector<int> dist_map(n);
    dijkstra_shortest_paths(network,a,distance_map(make_iterator_property_map(dist_map.begin(), get(vertex_index, network))));
    cout << dist_map[b] << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
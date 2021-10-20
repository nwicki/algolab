#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>


using namespace std;
using namespace boost;

struct edge_component_t {
    enum { num = 555 };
    typedef edge_property_tag kind;
} edge_component;

typedef adjacency_list<
        vecS,
        vecS,
        undirectedS,
        no_property,
        property<edge_component_t, std::size_t>
        > graph_u;
typedef graph_traits<graph_u>::vertex_descriptor vertex_u;
typedef graph_traits<graph_u>::edge_descriptor edge_u;
typedef graph_traits<graph_u>::edge_iterator edge_it;
typedef property_map<graph_u, edge_component_t>::type map_ec;

bool cmp_edges(const edge_u& a, const edge_u& b) {
    return a.m_source == b.m_source ? a.m_target < b.m_target : a.m_source < b.m_source;
}

void testcase() {
    int n,m; cin >> n >> m;
    graph_u islands(n);
    for(int i = 0; i < m; i++) {
        vertex_u e1,e2; cin >> e1 >> e2;
        add_edge(e1,e2,islands);
    }
    map_ec component = get(edge_component, islands);
    size_t num = biconnected_components(islands, component);
    vector<vector<edge_u>> component_edges(num,vector<edge_u>());
    edge_it ei, ei_end;
    for(tie(ei,ei_end) = edges(islands); ei != ei_end; ei++) {
        component_edges[component[*ei]].push_back(*ei);
    }
    vector<edge_u> result_edges;
    for(const auto& c : component_edges) {
        if(c.size() == 1) {
            edge_u e = c[0];
            if(e.m_source > e.m_target) {
                swap(e.m_source,e.m_target);
            }
            result_edges.push_back(e);
        }
    }
    sort(result_edges,cmp_edges);
    cout << result_edges.size() << endl;
    for(const auto& e : result_edges) {
        cout << e.m_source << " " << e.m_target << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
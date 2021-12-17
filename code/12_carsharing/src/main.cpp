#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, long,
        property<edge_residual_capacity_t, long,
            property<edge_reverse_t, traits::edge_descriptor,
                property<edge_weight_t, long>>>>> graph_t;
typedef graph_traits<graph_t>::edge_descriptor edge_t;
typedef graph_traits<graph_t>::vertex_descriptor vertex_t;

const bool PRINT = false;

void add_edge(graph_t& g, vertex_t u, vertex_t v, long capacity, long cost) {
    auto c_map = get(edge_capacity, g);
    auto r_map = get(edge_reverse, g);
    auto w_map = get(edge_weight, g);
    auto e = add_edge(u, v, g).first;
    auto rev_e = add_edge(v, u, g).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
}

class route {
public:
    size_t u;
    size_t v;
    size_t d;
    size_t a;
    long p;
};

void get_paths(const vector<vector<vertex_t>>& g, vertex_t source, vertex_t target, vector<vector<vertex_t>>& res) {
    deque<pair<vertex_t,size_t>> to_visit;
    vector<vector<vertex_t>> paths = vector<vector<vertex_t>>({ vector<vertex_t>({ source }) });
    to_visit.emplace_back(source,0);
    while(!to_visit.empty()) {
        auto current = to_visit.front();
        to_visit.pop_front();
        auto neighbours = g[current.first];
        for(auto n : neighbours) {
            paths.push_back(paths[current.second]);
            paths.back().push_back(n);
            to_visit.emplace_back(n, paths.size()-1);
        }
    }
    for(auto& p : paths) {
        if(p.back() != target) continue;
        res.push_back(p);
    }
}

void testcase() {
    size_t n, s; cin >> n >> s;
    vector<long> cars(s);
    for(auto& c : cars) {
        cin >> c;
    }
    vector<route> routes(n);
    vector<map<size_t,vertex_t>> indices(s);
    vector<vector<pair<size_t,vertex_t>>> indices_sorted(s);
    size_t vertices = 0;
    for(auto& r : routes) {
        cin >> r.u >> r.v >> r.d >> r.a >> r.p;
        r.u--; r.v--;
        if(indices[r.u].find(r.d) == indices[r.u].end()) {
            indices[r.u].insert(make_pair(r.d,vertices));
            indices_sorted[r.u].emplace_back(r.d, vertices);
            vertices++;
        }
        if(indices[r.v].find(r.a) == indices[r.v].end()) {
            indices[r.v].insert(make_pair(r.a,vertices));
            indices_sorted[r.v].emplace_back(r.a, vertices);
            vertices++;
        }
    }
    graph_t g(vertices);
    const long adjust = 100;
    const long MAX = 1L << 62;
    vertex_t source = add_vertex(g);
    vertex_t target = add_vertex(g);
    vector<vector<vertex_t>> path_graph(vertices+2);
    for(size_t i = 0; i < s; i++) {
        sort(indices_sorted[i].begin(), indices_sorted[i].end());
        vertex_t v = indices_sorted[i].front().second;
        path_graph[source].push_back(v);
        add_edge(g, source, v, cars[i], adjust);
        vertex_t u = indices_sorted[i].back().second;
        path_graph[u].push_back(target);
        add_edge(g, u, target, MAX, adjust);
    }
    for(auto& ind : indices_sorted) {
        for(size_t j = 0; j < ind.size()-1; j++) {
            vertex_t u = ind[j].second;
            vertex_t v = ind[j+1].second;
            path_graph[u].push_back(v);
            if(j+1 == ind.size()-1) {
                add_edge(g, u, v, MAX, adjust);
            }
            else {
                add_edge(g, u, v, MAX, adjust);
            }
        }
    }
    for(auto& r : routes) {
        size_t start = indices[r.u].find(r.d)->second;
        size_t end = indices[r.v].find(r.a)->second;
        path_graph[start].push_back(end);
        add_edge(g, start, end, 1, -r.p + adjust);
    }
    vector<vector<vertex_t>> paths;
    get_paths(path_graph, source, target, paths);
    vector<pair<long,vector<edge_t>>> edge_paths;
    edge_paths.reserve(paths.size());
    size_t max_length = 0;
    for(const auto& p : paths) {
        max_length = max(max_length, p.size()-1);
        vector<edge_t> edge_path;
        edge_path.reserve(p.size()-1);
        for(size_t i = 0; i < p.size()-1; i++) {
            auto res = edge(p[i], p[i+1], g);
            if(!res.second) {
                cout << "Edge somehow does not exist!" << endl;
                exit(1);
            }
            edge_path.push_back(res.first);
        }
        edge_paths.emplace_back(adjust * edge_path.size(),edge_path);
    }
    map<edge_t, long> weight_adjust;
    graph_traits<graph_t>::edge_iterator it, end;
    for(tie(it,end) = edges(g); it != end; it++) {
        weight_adjust[*it] = adjust;
    }
    long path_val = (long) max_length * adjust;
    map<edge_t, bool> handled;
    size_t finished = 0;
    for(tie(it,end) = edges(g); it != end; it++) {
        handled[*it] = false;
    }
    for(const auto& p : edge_paths) {
        if(path_val == p.first) {
             for(const auto& e : p.second) {
                 if(!handled[e]) {
                     handled[e] = true;
                     finished++;
                 }
            }
        }
    }
    auto w_map = get(edge_weight, g);
    auto r_map = get(edge_reverse, g);
    cout << "Handled: ";
    size_t n_edges = 0;
    for(tie(it,end) = edges(g); it != end; it++) {
        if(handled[*it]) {
            cout << *it << " " << w_map[*it] << " ";
        }
        n_edges++;
    }
    cout << endl;
    n_edges /= 2;
    cout << "Amount of edges: " << n_edges << endl;
    while(finished != n_edges) {
        for(auto& p : edge_paths) {
            vector<edge_t> update;
            for(auto& e : p.second) {
                if(!handled[e]) {
                    update.push_back(e);
                }
            }
            p.second = update;
        }
        for(auto& p : edge_paths) {
            auto e = p.second.front();
            if(p.second.size() == 1 && !handled[e]) {
                long val = path_val - p.first;
                w_map[e] += val;
                w_map[r_map[e]] -= val;
                handled[e] = true;
                cout << "New handled edge " << e << " with weight: " << w_map[e] << endl;
                finished++;
            }
        }
    }

//    push_relabel_max_flow(g, source, target);
//    cycle_canceling(g)
    successive_shortest_path_nonnegative_weights(g, source, target);
    cout << path_val - find_flow_cost(g) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
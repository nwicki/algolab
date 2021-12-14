#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <vector>
#include <queue>

using namespace boost;
using namespace std;

typedef adjacency_list_traits<vecS, vecS, directedS> traits;
typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_capacity_t, long,
            property<edge_residual_capacity_t, long, property<edge_reverse_t, traits::edge_descriptor,
                property <edge_weight_t, long>>>>> graph_t;
typedef graph_traits<graph_t>::edge_descriptor edge_t;
typedef graph_traits<graph_t>::vertex_descriptor vertex_t;
typedef graph_traits<graph_t>::edge_iterator edge_it_t;
typedef graph_traits<graph_t>::out_edge_iterator out_edge_it_t;

pair<edge_t,edge_t> add_edge(graph_t& g, vertex_t u, vertex_t v, long capacity, long cost) {
    auto c_map = get(edge_capacity, g);
    auto r_map = get(edge_reverse, g);
    auto w_map = get(edge_weight, g);
    const edge_t e = add_edge(u, v, g).first;
    const edge_t rev_e = add_edge(v, u, g).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
    return make_pair(e,rev_e);
}

void compute_80() {
    size_t n; cin >> n;
    graph_t days(n);
    vertex_t v_source = add_vertex(days);
    vertex_t v_target = add_vertex(days);
    long students = 0;
    for(vertex_t i = 0; i < n; i++) {
        long a, c; cin >> a >> c;
        add_edge(days, v_source, i, a, c);
    }
    for(vertex_t i = 0; i < n; i++) {
        long s, p; cin >> s >> p;
        students += s;
        add_edge(days, i, v_target, s, -p);
    }
    for(vertex_t i = 0; i < n-1; i++) {
        long v, e; cin >> v >> e;
        add_edge(days, i, i+1, v, e);
    }
    long flow = push_relabel_max_flow(days, v_source, v_target);
    cycle_canceling(days);
    if(flow != students) {
        cout << "im";
    }
    cout << "possible " << flow << " ";
    cout << -find_flow_cost(days) << endl;
}

void priority_q() {
    size_t n; cin >> n;
    vector<pair<long,long>> ca;
    ca.reserve(n);
    long total = 0L;
    for(size_t i = 0; i < n; i++) {
        long a, c; cin >> a >> c;
        ca.emplace_back(c, a);
    }
    vector<pair<long,long>> ps;
    ps.reserve(n);
    for(size_t i = 0L; i < n; i++) {
        long s, p; cin >> s >> p;
        total += s;
        ps.emplace_back(p, s);
    }
    vector<pair<long,long>> ev;
    ev.reserve(n);
    for(size_t i = 0L; i < n-1; i++) {
        long v, e; cin >> v >> e;
        ev.emplace_back(e, v);
    }
    priority_queue<pair<long,long>, vector<pair<long,long>>, greater<>> menus;
    long S = 0L;
    long P = 0L;
    for(size_t i = 0L; i < n; i++) {
        // First: cost, second: menus
        auto menu = ca[i];
        // First: price, second: students
        auto students = ps[i];
        // Push the menu from today as a possibility to feed the students
        menus.push(menu);
        while(!menus.empty() && 0L < students.second) {
            // Get the cheapest menu
            menu = menus.top();
            menus.pop();
            // Students we can feed with this menu
            long fed = min(menu.second, students.second);
            // Students that still need to be fed
            students.second -= fed;
            // Remaining menus
            menu.second -= fed;
            // Total amount of students we fed
            S += fed;
            // Total profit: students we fed times the profit per menu sold (price paid by student - cost of menu)
            P += fed * (students.first - menu.first);
            // If there are still menus remaining use the next day
            if(0L < menu.second) {
                menus.push(menu);
            }
        }
        // First: cost, second: space
        auto freezer = ev[i];
        priority_queue<pair<long,long>, vector<pair<long,long>>, greater<>> temp;
        while(!menus.empty() && 0L < freezer.second) {
            // Get the cheapest menus
            menu = menus.top();
            menus.pop();
            // Either we can place all menus in the freezer or the freezer determines the amount of menus
            menu.second = min(freezer.second, menu.second);
            // Freezer space is reduced by the amount of menus or will be zero
            freezer.second -= menu.second;
            // Using the cheapest menus from today the following day will additionally incur an energy cost
            menu.first += freezer.first;
            temp.push(menu);
        }
        menus = temp;
    }
    if(total != S) {
        cout << "im";
    }
    cout << "possible " << S << " " << P << endl;
}

void testcase() {
    size_t n; cin >> n;
    graph_t days(n);
    vertex_t v_source = add_vertex(days);
    vertex_t v_target = add_vertex(days);
    long students = 0;
    long adjust = 0;
    vector<edge_t> rev_edges;
    rev_edges.reserve(3 * n - 2);
    for(vertex_t i = 0; i < n; i++) {
        long a, c; cin >> a >> c;
        rev_edges.push_back(add_edge(days, v_source, i, a, c + adjust).second);
    }
    for(vertex_t i = 0; i < n; i++) {
        long s, p; cin >> s >> p;
        students += s;
        rev_edges.push_back(add_edge(days, i, v_target, s, -p + adjust).second);
    }
    for(vertex_t i = 0; i < n-1; i++) {
        long v, e; cin >> v >> e;
        rev_edges.push_back(add_edge(days, i, i+1, v, e + adjust).second);
    }
    push_relabel_max_flow(days, v_source, v_target);
    cycle_canceling(days);
    auto rc_map = get(edge_residual_capacity, days);
    edge_it_t it, end;
    for(tie(it, end) = edges(days); it != end; it++) {
        cout << "(" << source(*it, days) << "," << target(*it, days) << "): " << rc_map[*it] << endl;
    }
    long flow = 0;
    for(vertex_t i = 0; i < n; i++) {
        flow += rc_map[rev_edges[i]];
    }
    long used_flow = 0;
    for(auto& e : rev_edges) {
        used_flow += rc_map[e];
    }
    if(flow != students) {
        cout << "im";
    }
    cout << "possible " << flow << " ";
    cout << -(find_flow_cost(days) - used_flow * adjust) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
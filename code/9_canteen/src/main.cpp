#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
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
typedef graph_traits<graph_t>::out_edge_iterator out_edge_it_t;

void add_edge(graph_t& g, vertex_t u, vertex_t v, long capacity, long cost) {
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
}

void compute_80() {
    size_t n; cin >> n;
    graph_t days(n);
    vertex_t source = add_vertex(days);
    vertex_t target = add_vertex(days);
    long students = 0;
    for(vertex_t i = 0; i < n; i++) {
        long a, c; cin >> a >> c;
        add_edge(days, source, i, a, c);
    }
    for(vertex_t i = 0; i < n; i++) {
        long s, p; cin >> s >> p;
        students += s;
        add_edge(days, i, target, s, -p);
    }
    for(vertex_t i = 0; i < n-1; i++) {
        long v, e; cin >> v >> e;
        add_edge(days, i, i+1, v, e);
    }
    long flow = push_relabel_max_flow(days, source, target);
    if(flow != students) {
        cout << "im";
    }
    cout << "possible " << flow << " ";
    cycle_canceling(days);
    cout << -find_flow_cost(days) << endl;
}

void testcase() {
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
        auto menu = ca[i];
        auto price_student = ps[i];
        // Push the menu from today as a possibility to feed the students
        menus.push(menu);
        while(!menus.empty() && 0L < price_student.second) {
            // Get the cheapest menu
            menu = menus.top();
            menus.pop();
            // Students to be fed
            long students = price_student.second;
            // Remaining students we need to feed
            price_student.second = max(0L, price_student.second - menu.second);
            // Remaining menus we can distribute
            menu.second = max(0L, menu.second - students);
            // Students we fed
            long fed = students - price_student.second;
            // Total amount of students we fed
            S += fed;
            // Total profit: students we fed times the profit per menu sold (price paid by student - cost of menu)
            P += fed * (price_student.first - menu.first);
            // If there are still menus remaining use the next day
            if(0L < menu.second) {
                menus.push(menu);
            }
        }
        auto energy_volume = ev[i];
        priority_queue<pair<long,long>, vector<pair<long,long>>, greater<>> temp;
        while(!menus.empty() && 0L < energy_volume.second) {
            // Get the cheapest menus
            menu = menus.top();
            menus.pop();
            // Either we can place all menus in the freezer or the freezer determines the amount of menus
            menu.second = min(energy_volume.second, menu.second);
            // Freezer space is reduced by the amount of menus or will be zero
            energy_volume.second -= menu.second;
            // Using the cheapest menus from today the following day will additionally incur an energy cost
            menu.first += energy_volume.first;
            temp.push(menu);
        }
        menus = temp;
    }
    if(total != S) {
        cout << "im";
    }
    cout << "possible " << S << " " << P << endl;
}


int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
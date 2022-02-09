#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <boost/pending/disjoint_sets.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/bipartite.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<size_t,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;

void testcase() {
    int n, m; K::FT r; std::cin >> n >> m >> r;
    r = CGAL::square(r);

    std::vector<std::pair<K::Point_2,int>> stations;
    stations.reserve(n);
    for(int i = 0; i < n; i++) {
        int x, y; std::cin >> x >> y;
        stations.emplace_back(K::Point_2(x, y), i);
    }

    std::vector<std::pair<K::Point_2,K::Point_2>> clues;
    clues.reserve(m);
    for(int i = 0; i < m; i++) {
        int x1, y1, x2, y2; std::cin >> x1 >> y1 >> x2 >> y2;
        clues.emplace_back(K::Point_2(x1, y1), K::Point_2(x2, y2));
    }

    Triangulation t;
    t.insert(stations.begin(), stations.end());

    graph g(n);
    for(auto v1 = t.finite_vertices_begin(); v1 != t.finite_vertices_end(); v1++) {
        auto v2 = t.incident_vertices(v1);
        auto v1s = v2;
        do {
            if(t.is_infinite(v2)) continue;
            if(CGAL::squared_distance(v1->point(), v2->point()) <= r) {
                boost::add_edge(v1->info(), v2->info(), g);
                auto v3 = t.incident_vertices(v2);
                auto v2s = v3;
                do {
                    if(t.is_infinite(v3) || v1->info() == v3->info()) continue;
                    if(CGAL::squared_distance(v1->point(), v3->point()) <= r) boost::add_edge(v1->info(), v3->info(), g);
                } while(++v3 != v2s);
            }
        } while(++v2 != v1s);
    }

    if(!boost::is_bipartite(g)) {
        while(m--) std::cout << "n";
        std::cout << std::endl;
        return;
    }

    std::vector<int> component_map(n);
    boost::connected_components(g, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, g)));

    for(const auto& e : clues) {
        if(CGAL::squared_distance(e.first, e.second) <= r) { std::cout << "y"; continue; }
        auto va = t.nearest_vertex(e.first);
        auto vb = t.nearest_vertex(e.second);
        if(component_map[va->info()] == component_map[vb->info()] &&
           CGAL::squared_distance(va->point(), e.first) <= r &&
           CGAL::squared_distance(vb->point(), e.second) <= r) { std::cout << "y"; continue; }
        std::cout << "n";
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
}
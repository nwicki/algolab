#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<size_t,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;

typedef std::tuple<size_t,size_t,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;

int compute_f(const std::vector<int>& component_size, int k) {
    if(k == 1) return component_size[1];
    if(k == 2) return component_size[1] / 2 + component_size[2];
    if(k == 3) {
        int mult = std::min(component_size[1], component_size[2]);
        return (component_size[1] - mult) / 3 + (component_size[2] - mult) / 2 + mult + component_size[3];
    }
    int mult13 = std::min(component_size[1], component_size[3]);
    int one = component_size[1] - mult13;
    int two = component_size[2] + component_size[3] - mult13;
    return (one + 2 * (two & 1)) / 4 + two / 2 + mult13 + component_size[4];
}

void testcase() {
    int n, k, f0; K::FT s0; std::cin >> n >> k >> f0 >> s0;
    std::vector<std::pair<K::Point_2, size_t>> tents;
    tents.reserve(n);
    for(int i = 0; i < n; i++) {
        long x, y; std::cin >> x >> y;
        tents.emplace_back(K::Point_2(x, y), i);
    }
    Triangulation t;
    t.insert(tents.begin(), tents.end());
    EdgeV edges;
    edges.reserve(3*n);
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        size_t i1 = e->first->vertex((e->second+1)%3)->info();
        size_t i2 = e->first->vertex((e->second+2)%3)->info();
        edges.emplace_back(i1, i2, t.segment(e).squared_length());
    }
    std::sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
    });

    boost::disjoint_sets_with_storage<> uf(n);
    K::FT s = 0;
    EdgeV::const_iterator e = edges.begin();
    std::vector<int> component_count(n, 1);
    std::vector<int> component_size(k+1);
    component_size[1] = n;
    while(e != edges.end() && std::get<2>(*e) < s0) {
        size_t c1 = uf.find_set(std::get<0>(*e));
        size_t c2 = uf.find_set(std::get<1>(*e));
        if(c1 != c2) {
            if(compute_f(component_size, k) == f0) s = std::get<2>(*e);
            component_size[component_count[c1]]--;
            component_size[component_count[c2]]--;
            int size = std::min(component_count[c1] + component_count[c2], k);
            uf.link(c1, c2);
            component_count[uf.find_set(std::get<0>(*e))] = size;
            component_size[size]++;
        }
        e++;
    }
    int f = compute_f(component_size, k);
    int fc = f;
    while(e != edges.end() && f0 <= fc) {
        size_t c1 = uf.find_set(std::get<0>(*e));
        size_t c2 = uf.find_set(std::get<1>(*e));
        if(c1 != c2) {
            fc = compute_f(component_size, k);
            if(fc == f0) s = std::get<2>(*e);
            component_size[component_count[c1]]--;
            component_size[component_count[c2]]--;
            int size = std::min(component_count[c1] + component_count[c2], k);
            uf.link(c1, c2);
            component_count[uf.find_set(std::get<0>(*e))] = size;
            component_size[size]++;
        }
        e++;
    }
    std::cout << (long) s << " " << f << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
}
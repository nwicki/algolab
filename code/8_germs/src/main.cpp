#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<size_t,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<size_t,size_t,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

long compute_time(K::FT t) {
    K::FT x1 = (1.0 + CGAL::sqrt(1.0 + 4.0 * t)) / -2.0;
    K::FT x2 = (1.0 - CGAL::sqrt(1.0 + 4.0 * t)) / -2.0;
    K::FT res = CGAL::max(x1, x2);
    return (long) std::ceil(CGAL::to_double(CGAL::sqrt(res)));
}

void testcase(int n) {
    int left, bottom, right, top; std::cin >> left >> bottom >> right >> top;
    std::vector<std::pair<K::Point_2, size_t>> bacteria;
    bacteria.reserve(n);
    for(int i = 0; i < n; i++) {
        int x, y; std::cin >> x >> y;
        bacteria.emplace_back(K::Point_2(x,y), i);
    }
    Delaunay t;
    t.insert(bacteria.begin(), bacteria.end());
    EdgeV edges;
    edges.reserve(3*n); // there can be no more in a planar graph
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        size_t i1 = e->first->vertex((e->second+1)%3)->info();
        size_t i2 = e->first->vertex((e->second+2)%3)->info();
        if (i1 > i2) std::swap(i1, i2);
            edges.emplace_back(i1, i2, t.segment(e).squared_length());
    }
    std::sort(edges.begin(), edges.end(),
              [](const Edge& e1, const Edge& e2) -> bool { return std::get<2>(e1) < std::get<2>(e2); });
    boost::disjoint_sets_with_storage<> uf(n);
    size_t n_components = n;
    std::vector<std::pair<size_t,K::FT>> closest(n, std::make_pair(0, 1L << 62));
    for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
        size_t v1 = std::get<0>(*e);
        size_t v2 = std::get<1>(*e);
        size_t c1 = uf.find_set(v1);
        size_t c2 = uf.find_set(v2);
        if (c1 != c2) {
            K::FT dist = std::get<2>(*e);
            uf.link(c1, c2);
            if(dist < closest[v1].second) {
                closest[v1] = std::make_pair(v2, dist);
            }
            if(dist < closest[v2].second) {
                closest[v2] = std::make_pair(v1, dist);
            }
            if (--n_components == 1) break;
        }
    }
    std::vector<std::pair<K::FT, int>> time(n);
    for(int i = 0; i < n; i++) {
        K::Point_2 p = bacteria[i].first;
        K::FT tm = 0.0;
        K::FT dish_dist = CGAL::min(CGAL::min(p.x() - left, right - p.x()), CGAL::min(p.y() - bottom, top - p.y()));
        if(dish_dist != 0.0) {
            K::FT dist_time = dish_dist * dish_dist - 0.25;
            K::FT vertex_time = closest[i].second / 4.0 - 0.25;
            tm = CGAL::min(dist_time, vertex_time);
        }
        time[i] = std::make_pair(tm, i);
    }
    sort(time.begin(), time.end());
    std::cout << compute_time(time.begin()->first) << " " << compute_time(time[n/2].first) << " ";
    std::cout << compute_time(time.back().first) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int n; std::cin >> n;
    while(n) {
        testcase(n);
        std::cin >> n;
    }
}
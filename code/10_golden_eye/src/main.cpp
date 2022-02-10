#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_set>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef std::size_t Index;
typedef std::pair<K::Point_2,Index> IPoint;
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Delaunay;


void testcase() {
    int n, m; K::FT p; std::cin >> n >> m >> p;
    std::vector<IPoint> jammers;
    jammers.reserve(n);
    for (int i = 0; i < n; ++i) {
        int x, y; std::cin >> x >> y;
        jammers.emplace_back(K::Point_2(x, y), i);
    }

    Delaunay t;
    t.insert(jammers.begin(), jammers.end());

    EdgeV edges;
    edges.reserve(3 * n);
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        Index i1 = e->first->vertex((e->second + 1) % 3)->info();
        Index i2 = e->first->vertex((e->second + 2) % 3)->info();
        if (i1 > i2) std::swap(i1, i2);
        edges.emplace_back(i1, i2, t.segment(e).squared_length());
    }

    std::vector<std::pair<K::Point_2,K::Point_2>> missions;
    Index n_components = n + 2 * m;
    std::vector<std::unordered_set<Index>> uf_missions(n_components);
    missions.reserve(m);
    for(int i = 0; i < m; i++) {
        int sx, sy, tx, ty; std::cin >> sx >> sy >> tx >> ty;
        auto source = K::Point_2(sx, sy);
        auto target = K::Point_2(tx, ty);
        missions.emplace_back(source, target);
        auto vs = t.nearest_vertex(source);
        auto vt = t.nearest_vertex(target);
        edges.emplace_back(vs->info(), n + 2 * i, 4 * CGAL::squared_distance(vs->point(), source));
        edges.emplace_back(vt->info(), n + 2 * i + 1, 4 * CGAL::squared_distance(vt->point(), target));
        uf_missions[n + 2 * i].insert(i);
        uf_missions[n + 2 * i + 1].insert(i);
    }

    std::sort(edges.begin(), edges.end(), [](const Edge &e1, const Edge &e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
    });

    std::vector<K::FT> power(m);
    boost::disjoint_sets_with_storage<> uf(n_components);
    for(EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
        Index c1 = uf.find_set(std::get<0>(*e));
        Index c2 = uf.find_set(std::get<1>(*e));
        if (c1 != c2) {
            uf.link(c1, c2);
            Index cn = uf.find_set(std::get<0>(*e));
            Index co = cn == c1 ? c2 : c1;
            for(Index i : uf_missions[co]) {
                if(uf_missions[cn].find(i) != uf_missions[cn].end()) {
                    uf_missions[cn].erase(i);
                    power[i] = std::get<2>(*e);
                }
                else {
                    uf_missions[cn].insert(i);
                }
            }
            if (--n_components == 1) break;
        }
    }
    K::FT a = 0, b = 0;
    for(K::FT min_p : power) {
        a = CGAL::max(a, min_p);
        if(min_p <= p) {
            std::cout << "y";
            b = CGAL::max(b, min_p);
        }
        else std::cout << "n";
    }
    std::cout << std::endl;
    std::cout << (long) a << std::endl;
    std::cout << (long) b << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
}
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <boost/pending/disjoint_sets.hpp>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<size_t,K> vertex_base;
typedef CGAL::Triangulation_face_base_2<K> face_base;
typedef CGAL::Triangulation_data_structure_2<vertex_base, face_base> dt;
typedef CGAL::Delaunay_triangulation_2<K,dt> delaunay;

typedef tuple<size_t,size_t,K::FT> edge;

void testcase() {
    size_t n, m, k; K::FT s; cin >> n >> m >> s >> k;
    vector<pair<K::Point_2,size_t>> oaks;
    oaks.reserve(n);
    for(int i = 0; i < n; i++) {
        long x,y; cin >> x >> y;
        oaks.emplace_back(K::Point_2(x, y), i);
    }
    delaunay t;
    t.insert(oaks.begin(), oaks.end());

    vector<edge> edges;
    edges.reserve(3*n);
    for(auto e = t.finite_edges_begin(); e != t.finite_edges_end(); e++) {
        size_t i1 = e->first->vertex((e->second+1)%3)->info();
        size_t i2 = e->first->vertex((e->second+2)%3)->info();
        if (i1 > i2) swap(i1, i2);
        edges.emplace_back(i1, i2, t.segment(e).squared_length());
    }
    sort(edges.begin(), edges.end(), [](const edge& e1, const edge& e2) -> bool { return get<2>(e1) < get<2>(e2); });

    vector<pair<K::FT, size_t>> bones;
    bones.reserve(m);
    for(int i = 0; i < m; i++) {
        long x,y; cin >> x >> y;
        auto b = K::Point_2(x, y);
        auto obj = t.nearest_vertex(b);
        K::FT dist = 4 * CGAL::squared_distance(obj->point(), b);
        size_t oak = obj->info();
        bones.emplace_back(dist, oak);
    }
    sort(bones.begin(), bones.end(), greater<>());

    size_t max_bones = 0;
    size_t max_s = 0;
    K::FT min_q = 1L << 52;
    size_t n_components = n;
    vector<size_t> bones_per_union(n,0);
    boost::disjoint_sets_with_storage<> uf(n);
    for(auto& edge : edges) {
        size_t c1 = uf.find_set(get<0>(edge));
        size_t c2 = uf.find_set(get<1>(edge));
        K::FT length = get<2>(edge);
        while(!bones.empty() && bones.back().first <= length) {
            K::FT dist = bones.back().first;
            size_t oak = uf.find_set(bones.back().second);
            bones_per_union[oak] = bones_per_union[oak] + 1;
            if(dist <= s) {
                max_s = max(bones_per_union[oak],max_s);
            }
            if(k <= bones_per_union[oak] && dist < min_q) {
                min_q = dist;
            }
            bones.pop_back();
        }

        if (c1 != c2) {
            uf.link(c1, c2);
            size_t c3 = uf.find_set(get<0>(edge));
            bones_per_union[c3] = bones_per_union[c3] + bones_per_union[c3 == c1 ? c2 : c1];
            max_bones = max(bones_per_union[c3], max_bones);
            if(length <= s) {
                max_s = max(max_s, max_bones);
            }
            if(k <= max_bones && length < min_q) {
                min_q = length;
            }
            if (--n_components == 1) break;
        }
    }
    while(!bones.empty()) {
        K::FT dist = bones.back().first;
        size_t oak = uf.find_set(bones.back().second);
        size_t union_bones = bones_per_union[oak] + 1;
        bones_per_union[oak] = union_bones;
        if(dist <= s) {
            max_s = max(union_bones,max_s);
        }
        if(k <= union_bones && dist < min_q) {
            min_q = dist;
        }
        bones.pop_back();
    }
    cout << max_s << " " << (long) min_q << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
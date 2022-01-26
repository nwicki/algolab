#include <iostream>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;

void testcase() {
    int n, m; long r; std::cin >> n >> m >> r;
    std::vector<K::Point_2> trees;
    trees.reserve(n);
    for(int i = 0; i < n; i++) {
        long x, y; std::cin >> x >> y;
        trees.emplace_back(x, y);
    }
    Delaunay delaunay;
    delaunay.insert(trees.begin(), trees.end());
    std::priority_queue<std::pair<K::FT, Delaunay::Face_handle>> to_visit;
    for(auto f = delaunay.all_faces_begin(); f != delaunay.all_faces_end(); f++) {
        f->info() = 0;
        if(delaunay.is_infinite(f)) {
            to_visit.emplace(std::numeric_limits<K::FT>::max(), f);
        }
        else {
            to_visit.emplace(CGAL::squared_radius(f->vertex(0)->point(), f->vertex(1)->point(), f->vertex(2)->point()), f);
        }
    }
    while(!to_visit.empty()) {
        auto d = to_visit.top().first;
        auto f = to_visit.top().second;
        to_visit.pop();
        if (f->info() < d) {
            f->info() = d;
            for (int i = 0; i < 3; i++) {
                auto nf = f->neighbor(i);
                auto v1 = f->vertex((i+1)%3)->point();
                auto v2 = f->vertex((i+2)%3)->point();
                K::FT max_nf = CGAL::min(d, CGAL::squared_distance(v1, v2));
                if (nf->info() < max_nf) {
                    to_visit.emplace(max_nf, nf);
                }
            }
        }
    }
    for(int i = 0; i < m; i++) {
        long x, y, s; std::cin >> x >> y >> s;
        auto b = K::Point_2(x, y);
        auto v = delaunay.nearest_vertex(b)->point();
        K::FT required = s + r;
        bool clear = false;
        if(CGAL::squared_distance(b, v) >= CGAL::square(required)) {
            if(delaunay.locate(b)->info() >= CGAL::square(2 * required)) {
                clear = true;
            }
        }
        std::cout << (clear ? "y" : "n");
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
}
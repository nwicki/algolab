#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

void testcase(int n) {
    std::vector<K::Point_2> existing(n);
    for(auto& p : existing) {
        int x, y; std::cin >> x >> y;
        p = K::Point_2(x,y);
    }
    Triangulation t;
    t.insert(existing.begin(), existing.end());
    int m; std::cin >> m;
    for(int i = 0; i < m; i++) {
        int x, y; std::cin >> x >> y;
        K::Point_2 p(x,y);
        std::cout << (long) CGAL::to_double(CGAL::squared_distance(t.nearest_vertex(p)->point(), p)) << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int n; std::cin >> n;
    while(n) {
        testcase(n);
        std::cin >> n;
    }
}
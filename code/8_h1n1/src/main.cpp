#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <unordered_map>
#include <queue>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;

void testcase(int n) {
    std::vector<K::Point_2> points;
    points.reserve(n);
    for (int i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        points.emplace_back(x, y);
    }
    Delaunay t;
    t.insert(points.begin(), points.end());
    std::unordered_map<Delaunay::Face_handle, K::FT> escape_route;
    std::priority_queue<std::pair<K::FT, Delaunay::Face_handle>> to_visit;
    for(auto it = t.all_faces_begin(); it != t.all_faces_end(); it++) {
        escape_route[it] = 0;
        if (t.is_infinite(it)) {
            to_visit.emplace(1L << 53, it);
        }
    }
    while(!to_visit.empty()) {
        auto top = to_visit.top();
        auto old_dist = top.first;
        auto face = top.second;
        to_visit.pop();
        if(old_dist <= escape_route[face]) continue;
        escape_route[face] = old_dist;
        for (int i = 0; i < 3; i++) {
            auto neighbour = face->neighbor(i);
            auto v1 = face->vertex((i + 1) % 3);
            auto v2 = face->vertex((i + 2) % 3);
            K::FT new_dist = CGAL::squared_distance(v1->point(), v2->point()) / 4;
            new_dist = CGAL::min(new_dist, old_dist);
            to_visit.emplace(new_dist, neighbour);
        }
    }
    int m; std::cin >> m;
    for(int i = 0; i < m; i++) {
        int x, y; double d; std::cin >> x >> y >> d;
        K::Point_2 p(x,y);
        auto v = t.nearest_vertex(p);
        K::FT dist = CGAL::squared_distance(v->point(), p);
        bool escape = d <= dist && d <= escape_route[t.locate(p)];
        std::cout << (escape ? "y" : "n");
    }
    std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n;
  for (std::cin >> n; n != 0; std::cin >> n) testcase(n);
  return 0;
}
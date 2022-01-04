#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;

void testcase() {
    int m, n; std::cin >> m >> n;
    Delaunay delaunay;
    std::vector<std::pair<K::Point_2, K::FT>> participants;
    participants.reserve(m);
    for (int i = 0; i < m; ++i) {
        int x, y; K::FT d; std::cin >> x >> y >> d;
        participants.emplace_back(K::Point_2(x, y), d);
    }
    K::FT h; std::cin >> h;
    std::vector<Delaunay::Vertex_handle> lights;
    lights.reserve(n);
    for (int i = 0; i < n; i++) {
        int x, y; std::cin >> x >> y;
        lights.push_back(delaunay.insert(K::Point_2(x, y)));
    }
    std::unordered_map<Delaunay::Vertex_handle, std::vector<int>> nearest_participants;
    std::vector<int> alive;
    for(int i = 0; i < m; i++) {
        auto p = participants[i];
        auto nearest = delaunay.nearest_vertex(p.first);
        K::FT minimal_distance = h + p.second;
        minimal_distance *= minimal_distance;
        if(CGAL::squared_distance(nearest->point(), p.first) < minimal_distance)
            nearest_participants[nearest].push_back(i);
        else alive.push_back(i);
    }
    for(int i = n-1; -1 < i && alive.empty(); i--) {
        auto l = lights[i];
        delaunay.remove(l);
        for(auto& j : nearest_participants[l]) {
            auto p = participants[j];
            auto nearest = delaunay.nearest_vertex(p.first);
            K::FT minimal_distance = h + p.second;
            minimal_distance *= minimal_distance;
            if(CGAL::squared_distance(nearest->point(), p.first) < minimal_distance)
                nearest_participants[nearest].push_back(j);
            else alive.push_back(j);
        }
    }
    std::sort(alive.begin(), alive.end());
    for(int p : alive) {
        std::cout << p << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t t; std::cin >> t;
    while(t--) testcase();
}
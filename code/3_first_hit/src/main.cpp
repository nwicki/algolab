#include <iostream>
#include <vector>
#include <CGAL/number_utils.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <limits>
#include <algorithm>
#include <random>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

double floor_to_double(const K::FT& x) {
    double a = floor(CGAL::to_double(x));
    while(a > x) a -= 1;
    while(a+1 <= x) a += 1;
    return a;
}

void testcase(int n) {
    double x,y,a,b; cin >> x >> y >> a >> b;
    K::Ray_2 phileas(K::Point_2(x,y),K::Point_2(a,b));
    vector<K::Segment_2> segments(n);
    for(auto& segment : segments) {
        double r,s,t,u; cin >> r >> s >> t >> u;
        segment = K::Segment_2(K::Point_2(r,s),K::Point_2(t,u));
    }
    shuffle(segments.begin(),segments.end(),default_random_engine((unsigned) CGAL::to_double(segments[0].start().x())));
    K::FT min = numeric_limits<double>::max();
    bool not_found = true;
    K::Point_2 closest;
    K::Line_2 separating;
    K::Line_2 along_ray(phileas);
    for(const auto& segment : segments) {
        if(not_found || separating.has_on_positive_side(segment.start()) || separating.has_on_positive_side(segment.target())) {
            if(CGAL::do_intersect(phileas,segment)) {
                auto its = CGAL::intersection(phileas,segment);
                K::Point_2* point_of_intersection = boost::get<K::Point_2>(&*its);
                K::Point_2 poi;
                if(!point_of_intersection) {
                    K::Segment_2* segment_of_intersection = boost::get<K::Segment_2>(&*its);
                    K::FT distance_to_start = CGAL::squared_distance(segment_of_intersection->start(),phileas.start());
                    K::FT distance_to_target = CGAL::squared_distance(segment_of_intersection->target(),phileas.start());
                    if(distance_to_start < distance_to_target) {
                        poi = segment_of_intersection->start();
                    }
                    else {
                        poi = segment_of_intersection->target();
                    }
                }
                else {
                    poi = *point_of_intersection;
                }
                K::FT sq_dist = CGAL::squared_distance(poi,phileas.start());
                if(sq_dist < min) {
                    not_found = false;
                    min = sq_dist;
                    closest = poi;
                    separating = along_ray.perpendicular(closest);
                }
            }
        }
    }
    if(not_found) {
        cout << "no" << endl;
        return;
    }
    cout << (long) floor_to_double(closest.x()) << " " << (long) floor_to_double(closest.y()) << endl;
}

int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);
    int n; cin >> n;
    while(n) {
        testcase(n);
        cin >> n;
    }
}
#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <algorithm>
#include <random>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

void testcase(int n) {
    long x,y,a,b; cin >> x >> y >> a >> b;
    K::Ray_2 phileas(K::Point_2(x,y),K::Point_2(a,b));
    vector<K::Segment_2> segments(n);
    for(auto& segment : segments) {
        long r,s,t,u; cin >> r >> s >> t >> u;
        segment = K::Segment_2(K::Point_2(r,s),K::Point_2(t,u));
    }
    shuffle(segments.begin(),segments.end(),default_random_engine((unsigned long) &segments));
    for(const auto& s : segments) {
        if (CGAL::do_intersect(phileas, s)) {
            cout << "yes" << endl;
            return;
        }
    }
    cout << "no" << endl;
}

int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);
    int n; cin >> n;
    while(n) {
        testcase(n);
        cin >> n;
    }
}
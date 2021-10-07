#include <iostream>
#include <vector>
#include <CGAL/number_utils.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <algorithm>
#include <random>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

void testcase(int n) {
    vector<K::Point_2> citizens(n);
    for(auto& c : citizens) {
        double x,y; cin >> x >> y;
        c = K::Point_2(x,y);
    }
    shuffle(citizens.begin(), citizens.end(), default_random_engine((unsigned) CGAL::to_double(citizens[0].x())));
    Min_circle mc(citizens.begin(), citizens.end());
    cout << ceil(CGAL::sqrt(mc.circle().squared_radius())) << endl;
}

int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);
    int n; cin >> n;
    while(n) {
        testcase(n);
        cin >> n;
    }
}
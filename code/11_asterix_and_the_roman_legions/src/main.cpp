#include <iostream>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>
#include <cmath>
#include <CGAL/Gmpz.h>

using namespace std;
using namespace CGAL;

typedef long IT;
typedef Gmpz ET;
typedef Quadratic_program<IT> Program;
typedef Quadratic_program_solution<ET> Solution;

void testcase() {
    long x, y, n; cin >> x >> y >> n;
    Program lp(SMALLER, false, 0, false, 0);
    const int X = 0;
    const int Y = 1;
    const int D = 2;
    for(int i = 0; i < n; i++) {
        long a,b,c,v; cin >> a >> b >> c >> v;
        if(a * x + b * y + c < 0) {
            a = -a;
            b = -b;
            c = -c;
        }
        long d = (long) std::sqrt(a * a + b * b) * v;
        lp.set_a(X, i, -a); lp.set_a(Y, i, -b); lp.set_a(D, i, d); lp.set_b(i, c);
    }
    lp.set_l(D, true, 0);
    lp.set_c(D, -1);
    Solution s = solve_linear_program(lp, ET());
    cout << (long) -to_double(s.objective_value()) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
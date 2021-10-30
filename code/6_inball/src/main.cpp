#include <iostream>
#include <vector>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>
#include <CGAL/Gmpz.h>

using namespace std;
using namespace CGAL;

typedef int IT;
typedef Gmpz ET;

typedef Quadratic_program<IT> Program;
typedef Quadratic_program_solution<ET> Solution;

void testcase(int n) {
    int d; cin >> d;
    Program lp(SMALLER, false, 0, false, 0);
    for(int i = 0; i < n; i++) {
        int scale = 0;
        for(int j = 0; j < d; j++) {
            int aj; cin >> aj;
            lp.set_a(j, i, aj);
            scale += aj * aj;
        }
        lp.set_a(d, i, (int) std::sqrt(scale));
        int bi; cin >> bi;
        lp.set_b(i, bi);
    }
    lp.set_l(d, true, 0);
    lp.set_c(d, -1);
    Solution s = solve_linear_program(lp, ET());
    if(s.is_infeasible()) {
        cout << "none" << endl;
        return;
    }
    if(s.is_unbounded()) {
        cout << "inf" << endl;
        return;
    }
    cout << -(s.objective_value_numerator() / s.objective_value_denominator()) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int n; cin >> n;
    while(n != 0) {
        testcase(n);
        cin >> n;
    }
}
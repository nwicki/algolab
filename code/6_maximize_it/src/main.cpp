#include <iostream>
#include <vector>
#include <CGAL/Gmpz.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

using namespace std;
using namespace CGAL;

typedef int IT;
typedef Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void problem1(int a, int b) {
    Program lp(CGAL::SMALLER, true, 0, false, 0);
    const int X = 0;
    const int Y = 1;
    lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);
    lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a * b);
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);
    lp.set_c(X, a);
    lp.set_c(Y, -b);

    Solution s = solve_linear_program(lp, ET());
    if(s.is_infeasible()) {
        cout << "no" << endl;
        return;
    }
    if(s.is_unbounded()) {
        cout << "unbounded" << endl;
        return;
    }
    cout << -(s.objective_value_numerator() / s.objective_value_denominator()) << endl;
}

void problem2(int a, int b) {
    Program lp(CGAL::SMALLER, false, 0, true, 0);
    const int X = 0;
    const int Y = 1;
    const int Z = 2;
    lp.set_a(X, 0, -1); lp.set_a(Y, 0, -1); lp.set_b(0, 4);
    lp.set_a(X, 1, -4); lp.set_a(Y, 1, -2); lp.set_a(Z, 1, -1); lp.set_b(1, a * b);
    lp.set_a(X, 2, 1); lp.set_a(Y, 2, -1); lp.set_b(2, 1);
    lp.set_c(X, a);
    lp.set_c(Y, b);
    lp.set_c(Z, 1);

    Solution s = solve_linear_program(lp, ET());
    if(s.is_infeasible()) {
        cout << "no" << endl;
        return;
    }
    if(s.is_unbounded()) {
        cout << "unbounded" << endl;
        return;
    }
    cout << ceil(to_double(s.objective_value_numerator() / s.objective_value_denominator())) << endl;
}

void testcase(int p) {
    int a,b; cin >> a >> b;
    p == 1 ? problem1(a,b) : problem2(a,b);
}

int main() {
    int p; cin >> p;
    while(p != 0) {
        testcase(p);
        cin >> p;
    }
}
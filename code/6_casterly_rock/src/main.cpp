#include <iostream>
#include <vector>
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

using namespace std;
using namespace CGAL;

typedef Gmpz IT;
typedef Gmpz ET;

typedef Quadratic_program<IT> Program;
typedef Quadratic_program_solution<ET> Solution;

void testcase() {
    int n, m; long s; cin >> n >> m >> s;
    vector<pair<int,int>> nobles(n);
    for(auto& e : nobles) {
        cin >> e.first >> e.second;
    }
    vector<pair<int,int>> commoners(m);
    for(auto& e : commoners) {
        cin >> e.first >> e.second;
    }
    Program lp(SMALLER, false, 0, false, 0);
    const int BS = 0;
    const int CS = 1;
    int constraint = 0;
    // Cersei
    for(const auto& e : nobles) {
        lp.set_a(BS, constraint, -e.second); lp.set_a(CS, constraint, -1); lp.set_b(constraint, -e.first);
        constraint++;
    }
    for(const auto& e : commoners) {
        lp.set_a(BS, constraint, e.second); lp.set_a(CS, constraint, 1); lp.set_b(constraint, e.first);
        constraint++;
    }
    Solution sol = solve_linear_program(lp, ET());
    if(sol.is_infeasible()) {
        cout << "Yuck!" << endl;
        return;
    }
    if(s && s != -1) {
        // Tywin
        long s_adjusted = s;
        long bs_adjusted = 0L;
        long cs_adjusted = 0L;
        for(int i = 0; i < n; i++) {
            auto e = nobles[i];
            bs_adjusted += e.second;
            cs_adjusted++;
            s_adjusted += e.first;
        }
        for(int i = 0; i < m; i++) {
            auto e = commoners[i];
            bs_adjusted -= e.second;
            cs_adjusted--;
            s_adjusted -= e.first;
        }
        lp.set_a(BS, constraint, bs_adjusted); lp.set_a(CS, constraint, cs_adjusted); lp.set_b(constraint, s_adjusted);
        constraint++;
        sol = solve_linear_program(lp, ET());
    }
    if(!s || sol.is_infeasible()) {
        cout << "Bankrupt!" << endl;
        return;
    }
    // Jaime
    const int AW = 2;
    const int CW = 3;
    const int DW = 4;
    for(const auto& e : nobles) {
        lp.set_a(AW, constraint, e.first); lp.set_a(CW, constraint, 1); lp.set_a(DW, constraint, -1); lp.set_b(constraint, -e.second);
        constraint++;
        lp.set_a(AW, constraint, -e.first); lp.set_a(CW, constraint, -1); lp.set_a(DW, constraint, -1); lp.set_b(constraint, e.second);
        constraint++;
    }
    for(const auto& e : commoners) {
        lp.set_a(AW, constraint, e.first); lp.set_a(CW, constraint, 1); lp.set_a(DW, constraint, -1); lp.set_b(constraint, -e.second);
        constraint++;
        lp.set_a(AW, constraint, -e.first); lp.set_a(CW, constraint, -1); lp.set_a(DW, constraint, -1); lp.set_b(constraint, e.second);
        constraint++;
    }
    lp.set_a(DW, constraint, -1); lp.set_b(constraint, 0);
    constraint++;
    lp.set_c(DW, 1);
    // 90°
    lp.set_r(constraint, EQUAL); lp.set_a(AW, constraint, 1); lp.set_a(BS, constraint, -1); lp.set_b(constraint, 0);
    sol = solve_linear_program(lp, ET());
    cout << (long) ceil(to_double(sol.objective_value())) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
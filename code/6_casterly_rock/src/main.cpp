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
// 1 2 2 0 1 1 1 -1 -1 1 -1 -1
bool cersei(const vector<pair<int,int>>& nobles, const vector<pair<int,int>>& commoners) {
    Program lp(SMALLER, false, 0, false, 0);
    const int B = 0;
    const int C = 1;
    int constraint = 0;
    for(const auto& e : nobles) {
        lp.set_a(B, constraint, -e.second); lp.set_a(C, constraint, -1); lp.set_b(constraint, -e.first);
        constraint++;
    }
    for(const auto& e : commoners) {
        lp.set_a(B, constraint, e.second); lp.set_a(C, constraint, 1); lp.set_b(constraint, e.first);
        constraint++;
    }
    Solution s = solve_linear_program(lp, ET());
    if(s.is_infeasible()){
        cout << "Yuck!" << endl;
        return false;
    }
    return true;
}

// 1 4 0 -1 -1 1 -1 -1 1 1 1 -1
bool jaime(const vector<pair<int,int>>& nobles, const vector<pair<int,int>>& commoners) {
    Program lp(SMALLER, false, 0, false, 0);
    const int A = 0;
    const int C = 1;
    const int D = 2;
    int constraint = 0;
    for(const auto& e : nobles) {
        lp.set_a(A, constraint, e.first); lp.set_a(C, constraint, 1); lp.set_a(D, constraint, -1); lp.set_b(constraint, -e.second);
        constraint++;
        lp.set_a(A, constraint, -e.first); lp.set_a(C, constraint, -1); lp.set_a(D, constraint, -1); lp.set_b(constraint, e.second);
        constraint++;
    }
    for(const auto& e : commoners) {
        lp.set_a(A, constraint, e.first); lp.set_a(C, constraint, 1); lp.set_a(D, constraint, -1); lp.set_b(constraint, -e.second);
        constraint++;
        lp.set_a(A, constraint, -e.first); lp.set_a(C, constraint, -1); lp.set_a(D, constraint, -1); lp.set_b(constraint, e.second);
        constraint++;
    }
    lp.set_a(D, constraint, -1); lp.set_b(constraint, 0);
    lp.set_c(D, 1);
    Solution s = solve_linear_program(lp, ET());
    cout << (long) ceil(to_double(s.objective_value())) << endl;
    return true;
}

void compute_60(const vector<pair<int,int>>& nobles, const vector<pair<int,int>>& commoners, long s) {
    int n = (int) nobles.size();
    int m = (int) commoners.size();
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
    const int AW = lp.get_n();
    const int CW = AW+1;
    const int DW = CW+1;
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
    sol = solve_linear_program(lp, ET());
    cout << (long) ceil(to_double(sol.objective_value())) << endl;
}

void compute_80(const vector<pair<int,int>>& nobles, const vector<pair<int,int>>& commoners, long s) {
    int n = (int) nobles.size();
    int m = (int) commoners.size();
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
    const int AW = lp.get_n();
    const int CW = AW+1;
    const int DW = CW+1;
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
    compute_80(nobles,commoners,s);
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
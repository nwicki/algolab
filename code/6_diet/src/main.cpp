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

void testcase(int n, int m) {
    vector<pair<int,int>> nutrients(n);
    for(auto& e : nutrients) {
        cin >> e.first >> e.second;
    }
    vector<pair<int,vector<int>>> foods(m, pair<int,vector<int>>(0,vector<int>(n)));
    for(auto& e : foods) {
        cin >> e.first;
        for(auto& e1 : e.second) {
            cin >> e1;
        }
    }
    Program lp(SMALLER, true, 0, false, 0);
    for(int i = 0; i < n; i++) {
        int ii = 2 * i;
        for(int j = 0; j < m; j++) {
            lp.set_a(j, ii, -foods[j].second[i]);
            lp.set_a(j, ii + 1, foods[j].second[i]);
        }
        lp.set_b(ii, -nutrients[i].first);
        lp.set_b(ii + 1, nutrients[i].second);
    }
    for(int i = 0; i < m; i++) {
        lp.set_c(i, foods[i].first);
    }
    Solution s = solve_linear_program(lp, ET());
    if(s.is_infeasible()) {
        cout << "No such diet." << endl;
        return;
    }
    cout << ceil(to_double(s.objective_value_numerator() / s.objective_value_denominator())) << endl;
}

int main() {
    int n,m; cin >> n >> m;
    while(n != 0 || m != 0) {
        testcase(n,m);
        cin >> n >> m;
    }
}
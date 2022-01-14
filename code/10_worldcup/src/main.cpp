#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct object {
    K::Point_2 pos;
    long a = 0;
    long b = 0;
};

void testcase() {
    int n, m, c; std::cin >> n >> m >> c;
    std::vector<object> warehouses(n);
    for(auto& o : warehouses) {
        int x, y; std::cin >> x >> y >> o.a >> o.b;
        o.pos = K::Point_2(x, y);
    }
    std::vector<object> stadiums(m);
    for(auto& o : stadiums) {
        int x, y; std::cin >> x >> y >> o.a >> o.b;
        o.pos = K::Point_2(x, y);
    }
    std::vector<std::vector<int>> revenues(n, std::vector<int>(m));
    for(auto& w : revenues) {
        for(auto& s : w) {
            std::cin >> s;
        }
    }
    std::vector<object> contours(c);
    for(auto& o : contours) {
        int x, y; std::cin >> x >> y >> o.a;
        o.a *= o.a;
        o.pos = K::Point_2(x, y);
    }
    std::vector<std::vector<int>> contours_w(n);
    std::vector<std::vector<int>> contours_s(m);
    for(int i = 0; i < c; i++) {
        for(int j = 0; j < n; j++) {
            if(CGAL::squared_distance(warehouses[j].pos, contours[i].pos) < (double) contours[i].a)
                contours_w[j].push_back(i);
        }
        for(int j = 0; j < m; j++) {
            if(CGAL::squared_distance(stadiums[j].pos, contours[i].pos) < (double) contours[i].a)
                contours_s[j].push_back(i);
        }
    }
    std::vector<std::vector<int>> traversing(n, std::vector<int>(m));
    for(int i = 0; i < n; i++) {
        int w_size = (int) contours_w[i].size();
        for(int j = 0; j < m; j++) {
            int s_size = (int) contours_s[j].size();
            int count = 0;
            int ii = 0, jj = 0;
            while(ii < w_size && jj < s_size) {
                if(contours_w[i][ii] == contours_s[j][jj]) ii++, jj++;
                else if(contours_w[i][ii] < contours_s[j][jj]) count++, ii++;
                else count++, jj++;
            }
            count += w_size - ii +  s_size - jj;
            traversing[i][j] = count;
        }
    }
    Program lp (CGAL::SMALLER, true, 0, false, 0);
    int constraint = 0;
    for(int i = 0; i < m; i++) {
        lp.set_r(constraint, CGAL::EQUAL);
        for(int j = 0; j < n; j++) {
            lp.set_a(i * n + j, constraint, 1); // Demand
            lp.set_a(i * n + j, constraint + 1, (int) warehouses[j].b); // Upper limit
            lp.set_c(i * n + j, -100 * revenues[j][i] + traversing[j][i]); // Revenues
        }
        lp.set_b(constraint, (int) stadiums[i].a);
        lp.set_b(constraint + 1, (int) stadiums[i].b * 100);
        constraint += 2;
    }
    // Make sure to not use more beer than available
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            lp.set_a(i + j * n, constraint, 1);
        }
        lp.set_b(constraint, (int) warehouses[i].a);
        constraint++;
    }
    Solution s = CGAL::solve_linear_program(lp, ET());
    if(s.is_infeasible()) {
        std::cout << "RIOT!" << std::endl;
        return;
    }
    std::cout << (long) std::floor(CGAL::to_double(-s.objective_value() / 100)) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
}
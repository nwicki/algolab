#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

void get_ancestors(const std::vector<std::vector<int>>& offspring, const std::vector<int>& age, int current,
                   std::vector<std::pair<int,int>>& aggregate, std::vector<int>& last_offspring,
                   std::vector<std::vector<std::pair<int,int>>>& ancestors) {
    aggregate.emplace_back(age[current], current);
    size_t reset = aggregate.size();
    if(offspring[current].empty()) {
        ancestors[current] = aggregate;
        last_offspring[current] = current;
    }
    else {
        for(int off : offspring[current]) {
            get_ancestors(offspring, age, off, aggregate, last_offspring, ancestors);
            last_offspring[current] = last_offspring[off];
            aggregate.resize(reset);
        }
    }
}

void testcase() {
    int n, q; std::cin >> n >> q;
    std::unordered_map<std::string, int> index;
    std::vector<std::string> name(n);
    std::vector<int> age(n);
    for(int i = 0; i < n; i++) {
        std::cin >> name[i] >> age[i];
        index[name[i]] = i;
    }
    std::vector<std::vector<int>> offspring(n, std::vector<int>());
    std::vector<bool> is_root(n, true);
    for(int i = 1; i < n; i++) {
        std::string s, p; std::cin >> s >> p;
        offspring[index[p]].push_back(index[s]);
        is_root[index[s]] = false;
    }
    int root = -1;
    for(int i = 0; i < n; i++) {
        if(is_root[i]) {
            root = i;
            break;
        }
    }
    std::vector<std::vector<std::pair<int,int>>> ancestors(n);
    std::vector<int> last_offspring(n);
    std::vector<std::pair<int,int>> temp;
    get_ancestors(offspring, age, root, temp, last_offspring, ancestors);
    for(int i = 0; i < q; i++) {
        std::string s; int b; std::cin >> s >> b;
        auto& ancest = ancestors[last_offspring[index[s]]];
        auto res = std::upper_bound(ancest.begin(), ancest.end(), b,
                                    [](int b, const auto a) { return a.first <= b; });
        std::cout << name[res->second] << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}
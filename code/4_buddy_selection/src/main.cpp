#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <set>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/icl/map.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;
using namespace icl;

typedef adjacency_list<vecS,vecS,undirectedS> graph_t;
typedef graph_traits<graph_t>::vertex_descriptor vertex_t;


void compute_80() {
    int n,c,f; cin >> n >> c >> f;
    graph_t students(n);
    vector<set<string>> characteristics(n);
    for(int i = 0; i < n; i++) {
        set<string> current;
        for(int j = 0; j < c; j++) {
            string chara; cin >> chara;
            current.insert(chara);
        }
        characteristics[i] = current;
    }
    for(int i = 0; i < n; i++) {
        auto A = characteristics[i];
        for(int j = i + 1; j < n; j++) {
            auto B = characteristics[j];
            set<string> C;
            add_intersection(C,A,B);
            if(f < (int) C.size()) {
                add_edge(i,j,students);
            }
        }
    }
    vector<int> component(n);
    int num = connected_components(students,&component[0]);
    vector<vector<int>> separated(num);
    for(int i = 0; i < n; i++) {
        separated[component[i]].push_back(i);
    }
    for(int i = 0; i < num; i++) {
        int size = (int) separated[i].size();
        if(size % 2) {
            cout << "optimal" << endl;
            return;
        }
        graph_t current(size);
        for(int j = 0; j < size; j++) {
            int vj = separated[i][j];
            for(int k = j + 1; k < size; k++) {
                int vk = separated[i][k];
                if(edge(vj,vk,students).second) {
                    add_edge(j,k,current);
                }
            }
        }
        vector<vertex_t> mate(size);
        if(!checked_edmonds_maximum_cardinality_matching(current,&mate[0]) ||
            size / 2 != matching_size(current,&mate[0])
            ) {
            cout << "optimal" << endl;
            return;
        }
    }
    cout << "not optimal" << endl;
}

void testcase() {
    int n,c,f; cin >> n >> c >> f;
    graph_t students(n);
    vector<vector<string>> characteristics(n,vector<string>(c));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < c; j++) {
            cin >> characteristics[i][j];
        }
        sort(characteristics[i].begin(),characteristics[i].end(),less<string>());
    }
    for(int i = 0; i < n; i++) {
        auto A = characteristics[i];
        for(int j = i + 1; j < n; j++) {
            int common = 0;
            auto B = characteristics[j];
            auto itA = A.begin();
            auto itB = B.begin();
            itA = lower_bound(A.begin(),A.end(),*itB);
            if(itA == A.end()) {
                itA = A.begin();
                itB = lower_bound(B.begin(),B.end(),*itA);
            }
            while(common <= f && itB != B.end() && itA != A.end()) {
                if(*itB == *itA) {
                    common++;
                    itB++;
                    itA++;
                }
                else if(*itB < *itA) {
                    itB++;
                }
                else {
                    itA++;
                }
            }
            if(f < common) {
                add_edge(i,j,students);
            }
        }
    }
    vector<int> component(n);
    int num = connected_components(students,&component[0]);
    vector<vector<int>> separated(num);
    for(int i = 0; i < n; i++) {
        separated[component[i]].push_back(i);
    }
    for(int i = 0; i < num; i++) {
        int size = (int) separated[i].size();
        if(size % 2) {
            cout << "optimal" << endl;
            return;
        }
        graph_t current(size);
        for(int j = 0; j < size; j++) {
            int vj = separated[i][j];
            for(int k = j + 1; k < size; k++) {
                int vk = separated[i][k];
                if(edge(vj,vk,students).second) {
                    add_edge(j,k,current);
                }
            }
        }
        vector<vertex_t> mate(size);
        if(!checked_edmonds_maximum_cardinality_matching(current,&mate[0]) ||
            size / 2 != matching_size(current,&mate[0])
            ) {
            cout << "optimal" << endl;
            return;
        }
    }
    cout << "not optimal" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
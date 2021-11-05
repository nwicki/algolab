#define BOOST_ALLOW_DEPRECATED_HEADERS
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/graph_traits.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> traits;
typedef adjacency_list<vecS, vecS, directedS, no_property, property<
                        edge_capacity_t, long, property<
                                edge_residual_capacity_t, long, property<
                                        edge_reverse_t, traits::edge_descriptor>>>> graph_t;
typedef traits::vertex_descriptor vertex_t;

const char ALPHABET_START = 'A';
const char ALPHABET_END = 'Z';

void add_edge(graph_t& g, vertex_t u, vertex_t v, long c) {
    auto c_map = get(edge_capacity, g);
    auto r_map = get(edge_reverse, g);
    auto e = add_edge(u , v, g).first;
    auto rev_e = add_edge(v , u, g).first;
    c_map[e] = c;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
}

int alphabet_index(char c) {
    return c - ALPHABET_START;
}

void testcase() {
    int h, w; cin >> h >> w;
    string s; cin >> s;
    // Count occurrences of each letter
    vector<long> note(ALPHABET_END - ALPHABET_START + 1, 0);
    for(const char& c : s) {
        note[alphabet_index(c)]++;
    }
    // Store front
    vector<char> front(h * w);
    for(char& c : front) {
        cin >> c;
    }
    // Store back and reverse segments to match order of front
    vector<char> back(front.size());
    auto start = back.begin();
    for(auto it = start; it != back.end(); it++) {
        if(it - start == w) {
            reverse(start,it);
            start = it;
        }
        cin >> *it;
    }
    reverse(start,back.end());
    // Reduce occurrences for each letter where only one side is needed
    // (as we can just assign them without considering the opposite side)
    vector<bool> not_taken(front.size(), true);
    size_t cutoff = 0;
    bool changed = true;
    while(changed) {
        changed = false;
        for(size_t i = 0; i < front.size(); i++) {
            // Only use a letter if not yet used
            if(not_taken[i]) {
                int index_f = alphabet_index(front[i]);
                int index_b = alphabet_index(back[i]);
                bool f = note[index_f];
                bool b = note[index_b];
                // Reduce if only one side needed
                if(f ^ b) {
                    note[b ? index_b : index_f]--;
                    cutoff++;
                    not_taken[i] = false;
                    changed = true;
                }
            }
        }
    }
    // Count occurrences of parts where both sides are needed
    // (in 26 x 26 matrix where only upper triangular matrix is used)
    vector<vector<int>> both(note.size(), vector<int>(note.size(), 0));
    for(size_t i = 0; i < front.size(); i++) {
        int index_f = alphabet_index(front[i]);
        int index_b = alphabet_index(back[i]);
        if(note[index_f] && note[index_b]) {
            both[min(index_f, index_b)][max(index_f, index_b)]++;
        }
    }
    // Create a mapping of remaining letters in note to vertices
    vector<int> mapping(note.size(), -1);
    int index = 0;
    for(size_t i = 0; i < note.size(); i++) {
        if(note[i] != 0) {
            mapping[i] = index;
            index++;
        }
    }
    // Create graph where we add connections from source to vertex representing a part where front and back are used
    // Connect vertex to corresponding letters in note
    graph_t g(index);
    vertex_t source = add_vertex(g);
    vertex_t target = add_vertex(g);
    for(size_t i = 0; i < both.size(); i++) {
        for(size_t j = i; j < both.size(); j++) {
            int count = both[i][j];
            if(count) {
                vertex_t v = add_vertex(g);
                add_edge(g, source, v, count);
                add_edge(g, v, mapping[i], count);
                add_edge(g, v, mapping[j], count);
            }
        }
    }
    // Connect letters in note to target
    for(size_t i = 0; i < note.size(); i++) {
        if(note[i]) {
            add_edge(g, mapping[i], target, note[i]);
        }
    }
    // Used letters + the optimal assignment of remaining letters to letters in note
    // should match the amount of letters in the note.
    if(push_relabel_max_flow(g, source, target) + cutoff == s.size()) {
        cout << "Yes" << endl;
        return;
    }
    cout << "No" << endl;
}



int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
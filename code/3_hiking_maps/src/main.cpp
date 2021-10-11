#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <limits>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

class Map_Part {
public:
    vector<K::Segment_2> segments;
    vector<K::Line_2> lines;
    vector<int> legs;
    Map_Part() = default;
    explicit Map_Part(const vector<K::Segment_2>& seg) {
        segments = seg;
        for(size_t i = 0; i < segments.size(); i++) {
            lines.emplace_back(segments[i]);
            if(lines.back().has_on_negative_side(segments[(i + 1) % segments.size()].start())) {
                lines.back() = lines.back().opposite();
            }
        }
    }

    bool point_inside(const K::Point_2& p) {
        bool inside = true;
        for(size_t i = 0; i < lines.size() && inside; i++) {
            inside = (lines[i].has_on_positive_side(p) || lines[i].has_on(p));
        }
        return inside;
    }

    bool leg_inside(const K::Segment_2& s) {
        return point_inside(s.start()) && point_inside(s.target());
    }
};

void testcase() {
    int m,n; cin >> m >> n;

    int leg_count = m-1;
    vector<K::Segment_2> legs(leg_count);
    K::Point_2 start,target;
    int x,y; cin >> x >> y;
    start = K::Point_2(x,y);
    for(int i = 1; i < m; i++) {
        cin >> x >> y;
        target = K::Point_2(x,y);
        legs[i-1] = K::Segment_2(start,target);
        start = target;
    }

    vector<Map_Part> map_parts(n);
    for(int i = 0; i < n; i++) {
        vector<K::Segment_2> segments(3);
        for(int j = 0; j < 3; j++) {
            int x1,y1,x2,y2; cin >> x1 >> y1 >> x2 >> y2;
            segments[j] = K::Segment_2(K::Point_2(x1,y1),K::Point_2(x2,y2));
        }
        map_parts[i] = Map_Part(segments);
    }

    for(auto& mp : map_parts) {
        for(int i = 0; i < leg_count; i++) {
            if(mp.leg_inside(legs[i])) {
                mp.legs.push_back(i);
            }
        }
    }

    int count = 0;
    vector<int> tracker(leg_count,0);
    int beginning = 0;
    int min = numeric_limits<int>::max();
    for(int i = 0; i < n; i++) {
        for(int p : map_parts[i].legs) {
            if(!tracker[p]) {
                count++;
            }
            tracker[p]++;
        }
        while(count == leg_count) {
            int span = i - beginning + 1;
            if(span < min) {
                min = span;
            }
            for(int p : map_parts[beginning].legs) {
                tracker[p]--;
                if(!tracker[p]) {
                    count--;
                }
            }
            beginning++;
        }
    }
    cout << min << endl;
}

int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
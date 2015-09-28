#ifndef curve_h
#define curve_h

#include <vector>
#include <utility>
#include "Point.h"
//#include <map>

using namespace std;

class curve {
public:
    virtual vector<vector<Point>>& generate(const vector<Point> cpts)=0;
    virtual vector<Point> find_intersections(void) { return vector<Point>(); }
    void degree_inc() { degree++; };
    void degree_dec() { if (degree - 1) degree--; };
    unsigned int get_degree() const { return degree; };
    
//private:
    // hash map for later
    

protected:
    vector<vector<Point>> curves;
    unsigned int degree;
    float fidelity;
    
    static Point nevilles(const unsigned int d, const unsigned int begin, const vector<Point> cp, const vector<float> t_int, const float t) {
        if (d == 0)
            return cp[begin];
        
        return ((nevilles(d - 1, begin, cp, t_int, t) * (d - t)) +   \
                (nevilles(d - 1, begin + 1, cp, t_int, t) * t)) /      \
                (t_int[begin + d] - t_int[begin]);
    }
};

#endif /* curve_h */

#ifndef curve_h
#define curve_h

#include <vector>
#include <utility>
#include "Point.h"
#include <map>

using namespace std;

class curve {
public:
    virtual vector<vector<Point>>& generate(const vector<Point> cpts)=0;
    virtual vector<Point> find_intersections(void) { return vector<Point>(); }
    
    void degree_inc() { degree++; };
    void degree_dec() { if (degree - 1) degree--; };
    unsigned int get_degree() const { return degree; };
    
    void set_uniform_param() { parameterization = 0.0; };
    void set_centrip_param() { parameterization = 0.5; };
    void set_chordlen_param(){ parameterization = 1.0; };
    void set_unusual_param() { parameterization = 2.0; };
    
private:
    map<pair<unsigned int, unsigned int>,Point> hash;

    Point nevilles(const unsigned int d, const unsigned int begin, const vector<Point> cp, const vector<float> t_int, const float t) {
        if (d == 0)
            return cp[begin];
        
        map<pair<unsigned int, unsigned int>,Point>::iterator ii;
        ii = hash.find(pair<unsigned int, unsigned int>(d, begin));
        
        if (ii != hash.end())
            return ii->second;
        
        return hash[pair<unsigned int, unsigned int>(d, begin)] = \
                ((nevilles(d - 1, begin, cp, t_int, t) * (t_int[begin + d] - t)) +   \
                 (nevilles(d - 1, begin + 1, cp, t_int, t) * (t - t_int[begin]))) /  \
                (t_int[begin + d] - t_int[begin]);
    }

protected:
    vector<vector<Point>> curves;
    unsigned int degree;
    float fidelity;
    float parameterization;
    
    Point interpolate(const unsigned int d, const unsigned int begin, const vector<Point> cp, const vector<float> t_int, const float t) {
        hash = map<pair<unsigned int, unsigned int>,Point>();
        Point p = nevilles(d, begin, cp, t_int, t);
        hash.clear();
        return p;
    }
    
    vector<float> generate_ints(const vector<Point>& cpts, const float parameterization) {
        vector<float> ints;
        ints.push_back(0.0);
        for (int i = 0; i < degree; i++)
            ints.push_back(ints.back() + pow((cpts[i] - cpts[i + 1]).abs(), parameterization));
        return ints;
    }
};

#endif /* curve_h */

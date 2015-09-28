#ifndef curve_h
#define curve_h

#include <vector>
#include <utility>
#include "Point.h"
#include <map>

using namespace std;

class curve {
public:
    virtual vector<vector<Point>>& generate(const vector<Point> knots)=0;
    virtual vector<Point> find_intersections(void) const { return vector<Point>(); };
    virtual vector<Point> elevate_degree(const vector<Point>& cpts) { return cpts; };
    
    void degree_inc() { degree++; };
    void degree_dec() { if (degree - 1) degree--; };
    unsigned int get_degree() const { return degree; };
    
    void set_uniform_param() { parameterization = 0.0; };
    void set_centrip_param() { parameterization = 0.5; };
    void set_chordlen_param(){ parameterization = 1.0; };
    void set_unusual_param(const float param) { parameterization = param; };
    
private:
    map<pair<unsigned int, unsigned int>,Point> hash;

    Point nevilles(const unsigned int d, const unsigned int begin, const vector<Point>& control_points, const vector<float>& t_int, const float t) {
        // base case are the control points
        if (d == 0)
            return control_points[begin];
        
        // check if this bit has been calculated before
        auto p = pair<unsigned int, unsigned int>(d, begin);
        auto ii = hash.find(p);
        
        // if it has, don't recalculate it!
        if (ii != hash.end())
            return ii->second;
        
        // otherwise, yeah go ahead and calculate it
        return hash[p] = \
                ((nevilles(d - 1, begin, control_points, t_int, t) * (t_int[begin + d] - t))  + \
                 (nevilles(d - 1, begin + 1, control_points, t_int, t) * (t - t_int[begin]))) / \
                 (t_int[begin + d] - t_int[begin]);
    }
    
    Point bezier_pyramid(unsigned int d, unsigned int begin, const vector<Point>& cp, const float t) {
        if (d == 0)
            return cp[begin];
        
        auto p = pair<unsigned int, unsigned int>(d, begin);
        auto ii = hash.find(p);
        
        if (ii != hash.end())
            return ii->second;
        
        return hash[p] = (bezier_pyramid(d - 1, begin, cp, t) * (1.0 - t)) + (bezier_pyramid(d - 1, begin + 1, cp, t) * t);
    }

protected:
    vector<vector<Point>> curves;
    unsigned int degree;
    float fidelity;
    float parameterization;
    
    // used by some curve types to generate the intervals between knots
    vector<float> generate_ints(const vector<Point>& knots, const float parameterization) {
        vector<float> intervals;
        intervals.push_back(0.0);
        for (int i = 0; i < degree; i++)
            intervals.push_back(intervals.back() + pow((knots[i] - knots[i + 1]).abs(), parameterization));
        return intervals;
    }
    
    vector<vector<Point>> divvy_knots(const vector<Point> knots) const {
        vector<vector<Point>> parts;
        
        for (unsigned int segment = 0; segment + degree < knots.size(); segment += (degree + 1))
            parts.push_back(vector<Point>(knots.begin() + segment, knots.begin() + segment + degree + 1));

        return parts;
    }
    
    // wrapper function for nevilles algorithm, needed to clear
    // the hash so there are no erronious recalls
    Point interpolate(const vector<Point>& control_points, const vector<float>& t_int, const float t) {
        hash = map<pair<unsigned int, unsigned int>,Point>();
        auto p = nevilles(degree, 0, control_points, t_int, t);
        hash.clear();
        return p;
    }
    
    // same for bezier curves
    Point bezier_pyramid(const vector<Point>& control_points, const float t) {
        hash = map<pair<unsigned int, unsigned int>,Point>();
        auto p = bezier_pyramid(degree, 0, control_points, t);
        hash.clear();
        return p;
    }
    
};

#endif /* curve_h */

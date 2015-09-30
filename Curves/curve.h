#ifndef curve_h
#define curve_h

#include <vector>
#include <utility>
#include "Point.h"
#include <map>

using namespace std;

class curve {
public:
    virtual vector<vector<Point>>& generate(const vector<Point> c_points)=0;
    virtual vector<Point> find_intersections(void) { return vector<Point>(); };
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

    Point neville(const unsigned int d, const unsigned int begin, const vector<Point>& c_points, const vector<float>& knots, const float t) {
        // base case are the control points
        if (d == 0)
            return c_points[begin];
        
        // check if this bit has been calculated before
        auto p = pair<unsigned int, unsigned int>(d, begin);
        auto ii = hash.find(p);
        
        // if it has, don't recalculate it!
        if (ii != hash.end())
            return ii->second;
        
        // otherwise, yeah go ahead and calculate it
        return hash[p] = \
                ((neville(d - 1, begin, c_points, knots, t) * (knots[begin + d] - t)) + \
                (neville(d - 1, begin + 1, c_points, knots, t) * (t - knots[begin]))) / \
                (knots[begin + d] - knots[begin]);
    }
    
    Point decasteljau(const unsigned int d, const unsigned int begin, const vector<Point>& c_points, const float t) {
        if (d == 0)
            return c_points[begin];
        
        auto p = pair<unsigned int, unsigned int>(d, begin);
        auto ii = hash.find(p);
        
        if (ii != hash.end())
            return ii->second;
        
        return hash[p] = (decasteljau(d - 1, begin, c_points, t) * (1.0 - t)) + (decasteljau(d - 1, begin + 1, c_points, t) * t);
    }
    
    Point deboor(const unsigned int d, const unsigned int begin, const vector<Point>& c_points, const float t) {
        if (d == 0)
            return c_points[begin];
        
        auto p = pair<unsigned int, unsigned int>(d, begin);
        auto ii = hash.find(p);
        
        if (ii != hash.end())
            return ii->second;
        
        // return (left * (begin + degree - t) + right * (t - (d - 1 )) / d
        return hash[p] = ((deboor(d - 1, begin, c_points, t) * (begin + degree - t)) + \
                          (deboor(d - 1, begin + 1, c_points, t) * (t - (d + begin - 1)))) / d;
    }
    
    Point cm_deboor(const unsigned int d, const unsigned int begin, const vector<Point>& c_points, const vector<float>& knots, const float t) {
        auto p = pair<unsigned int, unsigned int>(d + degree, begin);
        auto ii = hash.find(p);
        
        if (ii != hash.end())
            return ii->second;
        
        if (d == 1)
            return hash[p] = \
                ((neville(degree - 1, begin, c_points, knots, t) * (knots[begin + degree] - t)) +    \
                (neville(degree - 1, begin + 1, c_points, knots, t) * (t - knots[d + begin - 1]))) / \
                (knots[begin + degree] - knots[d + begin - 1]);
        
        return hash[p] = \
                ((cm_deboor(d - 1, begin, c_points, knots, t) * (knots[begin + degree] - t)) +       \
                (cm_deboor(d - 1, begin + 1, c_points, knots, t) * (t - knots[d + begin - 1]))) /    \
                (knots[begin + degree] - knots[d + begin - 1]);
    }
    
protected:
    vector<vector<Point>> curves;
    unsigned int degree;
    float fidelity;
    float parameterization;
    
    // used by some curve types to generate the intervals between knots
    static vector<float> generate_ints(const vector<Point>& c_points, const unsigned int size, const float parameterization) {
        vector<float> knots;
        knots.push_back(0.0);
        for (unsigned int i = 0; i < size; i++)
            knots.push_back(knots.back() + pow((c_points[i] - c_points[i + 1]).abs(), parameterization));
        return knots;
    }
    
    // groups up the control points for each bezier and lagrange curve
    vector<vector<Point>> divvy_points(const vector<Point> c_points) const {
        vector<vector<Point>> parts;
        
        for (unsigned int segment = 0; segment + degree < c_points.size(); segment += (degree + 1))
            parts.push_back(vector<Point>(c_points.begin() + segment, c_points.begin() + segment + degree + 1));

        return parts;
    }
    
    // wrapper function for nevilles algorithm, needed to clear
    // the hash so there are no erronious recalls
    Point neville(const vector<Point>& c_points, const vector<float>& knots, const float t) {
        hash = map<pair<unsigned int, unsigned int>,Point>();
        auto p = neville(degree, 0, c_points, knots, t);
        hash.clear();
        return p;
    }
    
    // same for bezier curves
    Point decasteljau(const vector<Point>& c_points, const float t) {
        hash = map<pair<unsigned int, unsigned int>,Point>();
        auto p = decasteljau(degree, 0, c_points, t);
        hash.clear();
        return p;
    }
    
    // and for b-splines
    Point deboor(const vector<Point>& c_points, const unsigned int piece, const float t) {
        hash = map<pair<unsigned int, unsigned int>,Point>();
        auto p = deboor(degree, piece, c_points, t);
        hash.clear();
        return p;
    }
    
    // and lastly catmull-rom curves
    Point cm_deboor(const vector<Point>& c_points, const vector<float>& knots, const unsigned int piece, const float t) {
        hash = map<pair<unsigned int, unsigned int>,Point>();
        auto p = cm_deboor(degree, piece, c_points, knots, t);
        hash.clear();
        return p;
    }
    
};

#endif /* curve_h */

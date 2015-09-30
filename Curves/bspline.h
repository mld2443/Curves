
#ifndef bspline_h
#define bspline_h

#include "curve.h"

class bspline : public curve {
public:
    bspline(const int d = 1, const float f = 1.0/100.0, const float p = 0.0) {
        degree = d;
        fidelity = f;
        parameterization = p;
        curve = vector<Point>();
    }
    
    vector<Point>& generate(const vector<Point>& control_points = vector<Point>()) {
        if (control_points.size())
            c_points = control_points;
        curve.clear();
        
        for (unsigned int piece = 0; piece < c_points.size() - degree; piece++) {
            for (float t = piece + degree - 1; t < piece + degree; t += fidelity)
                curve.push_back(deboor(piece, t));
        }
        
        return curve;
    }
    
    curvetype get_type() const {
        return curvetype::bspline;
    }
};

#endif /* bspline_h */

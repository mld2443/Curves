
#ifndef bspline_h
#define bspline_h

#include "curve.h"

class bspline : public curve {
public:
    bspline(const int d = 1, const float f = 1.0/100.0) {
        degree = d;
        fidelity = f;
        parameterization = 0.0;
        curves = vector<vector<Point>>();
    }
    
    vector<vector<Point>>& generate(const vector<Point> c_points) {
        curves.clear();
        vector<Point> curve;
        
        for (unsigned int piece = 0; piece < c_points.size() - degree; piece++) {
            for (float t = piece + degree - 1; t < piece + degree; t += fidelity)
                curve.push_back(deboor(c_points, piece, t));
        }
        curves.push_back(curve);
        
        return curves;
    }
};

#endif /* bspline_h */

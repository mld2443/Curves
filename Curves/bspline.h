
#ifndef bspline_h
#define bspline_h

#include "curve.h"

class bspline : public curve {
public:
    bspline(const int d = 1, const float f = 1.0/100.0) {
        degree = d;
        fidelity = f;
        curves = vector<vector<Point>>();
    }
    
    vector<vector<Point>>& generate(const vector<Point> cpts) {
        //TODO: all of this
        return curves;
    }
};

#endif /* bspline_h */

#ifndef catmullrom_h
#define catmullrom_h

#include "curve.h"

class catmullrom : public curve {
public:
    catmullrom(const int s = 0, const float f = 1.0/100.0) {
        degree = s;
        fidelity = f;
        parameterization = 0.0;
        curves = vector<vector<Point>>();
    }
    
    vector<vector<Point>>& generate(const vector<Point> knots) {
        //TODO: all of this
        return curves;
    }
};

#endif /* catmullrom_h */

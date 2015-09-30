#ifndef lagrange_h
#define lagrange_h

#include <math.h>
#include "curve.h"

class lagrange : public curve {
public:
    lagrange(const int d = 1, const float f = 1.0/100.0, const float p = 0.0) {
        degree = d;
        fidelity = f;
        parameterization = p;
        curve = vector<Point>();
    }
    
    vector<Point>& generate(const vector<Point>& control_points = vector<Point>()) {
        if (control_points.size())
            c_points = control_points;
        curve.clear();
        
        auto knots = generate_ints(c_points, degree, parameterization);
        
        float step = (knots[degree] / (float)degree) * fidelity;
        for (float t = knots[0]; t <= knots[degree]; t += step)
            curve.push_back(neville(knots, t));
        
        // force last endpoint interpolation
        curve.push_back(c_points.back());
        
        return curve;
    }
    
    curvetype get_type() const {
        return curvetype::lagrange;
    }
};

#endif /* lagrange_h */

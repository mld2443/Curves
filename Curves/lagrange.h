#ifndef lagrange_h
#define lagrange_h

#include <math.h>
#include "curve.h"

class lagrange : public curve {
public:
    lagrange(const int d = 1, const float f = 1.0/100.0) {
        degree = d;
        fidelity = f;
        parameterization = 0.0;
        curves = vector<vector<Point>>();
    }
    
    vector<vector<Point>>& generate(const vector<Point> knots) {
        curves.clear();
        
        auto parts = divvy_knots(knots);
        for (auto &part : parts) {
            vector<Point> curve;
            
            auto t_int = generate_ints(part, parameterization);
            
            float step = (t_int[degree] / (float)degree) * fidelity;
            for (float t = t_int[0]; t <= t_int[degree]; t += step)
                curve.push_back(interpolate(part, t_int, t));
            
            // force last endpoint interpolation
            curve.push_back(part.back());
            curves.push_back(curve);
        }
        return curves;
    }
};

#endif /* lagrange_h */

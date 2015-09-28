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
    
    vector<vector<Point>>& generate(const vector<Point> cpts) {
        curves.clear();
        
        for (unsigned int segment = 0; segment + degree < cpts.size(); segment += (degree + 1)) {
            vector<Point> curve, segment_cpts = vector<Point>(cpts.begin() + segment, cpts.end() + segment + degree);
            
            vector<float> t_int = generate_ints(segment_cpts, parameterization);
            
            float step = (t_int[degree] / (float)degree) * fidelity;
            for (float t = t_int[0]; t < t_int[degree]; t += step) {
                curve.push_back(interpolate(degree, 0, segment_cpts, t_int, t));
            }
            
            curves.push_back(curve);
        }
        return curves;
    }
};

#endif /* lagrange_h */

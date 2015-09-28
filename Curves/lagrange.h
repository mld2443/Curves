#ifndef lagrange_h
#define lagrange_h

#include "curve.h"

class lagrange : public curve {
public:
    lagrange(const int d = 1, const float f = 1.0/100.0) {
        degree = d;
        fidelity = f;
        curves = vector<vector<Point>>();
    }
    
    vector<vector<Point>>& generate(const vector<Point> cpts) {
        vector<float> t_int;
        for (int i = 0; i < cpts.size(); i++)
            t_int.push_back((float)i);
        
        unsigned int piece = 0;
        for (unsigned int segment = 0; segment < cpts.size() ; segment++) {
            vector<Point> curve = vector<Point>();
            
            for (float t = t_int[segment]; t < t_int[segment + 1]; t += fidelity) {
                curve.push_back(nevilles(degree, 0, cpts, t_int, t));
            }
            
            if (++piece == degree) {
                curves.push_back(curve);
                segment++;
                piece = 0;
            }
        }
        return curves;
    }
};

#endif /* lagrange_h */

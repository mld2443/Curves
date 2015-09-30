
#ifndef catmullrom_h
#define catmullrom_h

#include "curve.h"

class catmullrom : public curve {
public:
    catmullrom(const int s = 1, const float f = 1.0/100.0) {
        degree = s;
        fidelity = f;
        parameterization = 0.0;
        curves = vector<vector<Point>>();
    }
    
    vector<vector<Point>>& generate(const vector<Point> c_points) {
        curves.clear();
        
        vector<Point> curve;
        
        auto knots = generate_ints(c_points, c_points.size() - 1, parameterization);
        
        float step = (knots[degree] / (float)degree) * fidelity;
        int justincase = 0;
        for (unsigned int piece = degree - 1; piece < c_points.size() - degree; piece++) {
            for (float t = knots[piece]; t <= knots[piece + 1]; t += step)
                curve.push_back(cm_deboor(c_points, knots, justincase, t));
            justincase++;
        }
        curves.push_back(curve);
        
        return curves;
    }
};

#endif /* catmullrom_h */

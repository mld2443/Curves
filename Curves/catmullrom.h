
#ifndef catmullrom_h
#define catmullrom_h

#include "curve.h"

class catmullrom : public curve {
public:
    catmullrom(const int s = 1, const float f = 1.0/100.0, const float p = 0.0) {
        degree = s;
        fidelity = f;
        parameterization = p;
        curve = vector<Point>();
    }
    
    vector<Point>& generate(const vector<Point>& control_points = vector<Point>()) {
        if (control_points.size())
            c_points = control_points;
        curve.clear();
        
        auto knots = generate_ints(c_points, c_points.size() - 1, parameterization);
        
        float step = (knots[degree] / (float)degree) * fidelity;
        int begin = 0;
        for (unsigned int piece = degree - 1; piece < c_points.size() - degree; piece++) {
            for (float t = knots[piece]; t <= knots[piece + 1]; t += step)
                curve.push_back(cm_deboor(knots, begin, t));
            begin++;
        }
        
        return curve;
    }
    
    curvetype get_type() const {
        return curvetype::catmullrom;
    }
};

#endif /* catmullrom_h */

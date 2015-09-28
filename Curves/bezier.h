#ifndef bezier_h
#define bezier_h

#include "curve.h"

class bezier : public curve {
public:
    bezier(const int d = 1, const float f = 1.0/100.0) {
        degree = d;
        fidelity = f;
        parameterization = 0.0;
        curves = vector<vector<Point>>();
    }
    
    vector<vector<Point>>& generate(const vector<Point> cpts) {
        // TODO: everything
        return curves;
    }
    
    vector<Point> find_intersections(void){
        vector<Point> results = vector<Point>();
        //TODO: intersections
        return results;
    }
    
private:
    pair<Point, Point> bounding_box(const vector<Point> c) {
        pair<Point, Point> box = pair<Point, Point>();
        //TODO: compute bounding box
        return box;
    }
};

#endif /* bezier_h */

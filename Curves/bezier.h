#ifndef bezier_h
#define bezier_h

#include "curve.h"

class bezier : public curve {
public:
    bezier(const int d = 1, const float f = 1.0/1000.0) {
        degree = d;
        fidelity = f;
        parameterization = 0.0;
        curves = vector<vector<Point>>();
    }
    
    vector<vector<Point>>& generate(const vector<Point> knots) {
        curves.clear();
        
        vector<vector<Point>> parts = divvy_knots(knots);
        for (auto &part : parts) {
            vector<Point> curve;
            
            for (float t = 0.0; t <= 1.0; t += fidelity)
                curve.push_back(bezier_pyramid(part, t));
            
            curve.push_back(part.back());
            curves.push_back(curve);
        }
        return curves;
    }
    
    vector<Point> find_intersections(void) {
        vector<Point> results = vector<Point>();
        //TODO: intersections
        return results;
    }
    
    vector<Point> elevate_degree(const vector<Point>& cpts) {
        vector<Point> newpts;
        
        vector<vector<Point>> parts = divvy_knots(cpts);
        for (auto &part : parts) {
            newpts.push_back(part.front());
            for (int i = 1; i < part.size(); i++)
                newpts.push_back(part[i] * (1 - ((float)i / (float)(degree + 1))) + part[i - 1] * ((float)i / (float)(degree + 1)));
            newpts.push_back(part.back());
        }
        
        degree_inc();
        return newpts;
    }
    
private:
    pair<Point, Point> bounding_box(const vector<Point> c) {
        pair<Point, Point> box = pair<Point, Point>();
        //TODO: compute bounding box
        return box;
    }
};

#endif /* bezier_h */

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
    
    vector<Point> find_intersections(void) {
        vector<Point> results;
        vector<pair<pair<Point, Point>, vector<Point>*>> boxes;
        
        // create a list of all the curves' bounding boxes, with references to the
        // curves for further searching
        for (auto &curve : curves) {
            boxes.push_back(pair<pair<Point, Point>, vector<Point>*>(bounding_box(curve),&curve));
        }
        
        // compare all curves to all others
        for (auto left = boxes.begin(); left != boxes.end(); left++) {
            for (auto right = left + 1; right != boxes.end(); right++) {
                // if they overlap at all, investigate
                if (overlap(left->first, right->first)) {
                    auto pts = intersect_search(*(left->second), *(right->second));
                    
                    // if investigation finds points, add them
                    if (pts.size())
                        results.insert(results.end(), pts.begin(), pts.end());
                }
            }
        }
        
        return results;
    }
    
private:
    static vector<Point> intersect_search(const vector<Point>& left, const vector<Point>& right) {
        vector<Point> intersections;
        // actual subdivision
        return intersections;
    }
    
    static bool overlap(const pair<Point, Point>& crvA, const pair<Point, Point>& crvB) {
        if (crvA.first.x < crvB.second.x && crvA.second.x > crvB.first.x &&
            crvA.first.y < crvB.second.y && crvA.second.y > crvB.first.y)
            return true;
        return false;
    }
    
    static pair<Point, Point> bounding_box(const vector<Point> curve) {
        float minx = curve[0].x, maxx = curve[0].x, miny = curve[0].y, maxy = curve[0].y;
        for (auto &pt : curve) {
            if (pt.x < minx) minx = pt.x;
            else if (pt.x > maxx) maxx = pt.x;
            if (pt.y < miny) miny = pt.y;
            else if (pt.y < maxy) maxy = pt.y;
        }
        return pair<Point, Point>(Point(minx, miny), Point(maxx, maxy));
    }
};

#endif /* bezier_h */

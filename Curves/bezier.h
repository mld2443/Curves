#ifndef bezier_h
#define bezier_h

#include "curve.h"

#define ANGLE_TOLERANCE 0.02

class bezier : public curve {
public:
    bezier(const int d = 1, const float f = 1.0/1000.0) {
        degree = d;
        fidelity = f;
        parameterization = 0.0;
        curves = vector<vector<Point>>();
    }
    
    vector<vector<Point>>& generate(const vector<Point> c_points) {
        curves.clear();
        
        vector<vector<Point>> parts = divvy_points(c_points);
        for (auto &part : parts) {
            vector<Point> curve;
            
            for (float t = 0.0; t <= 1.0; t += fidelity)
                curve.push_back(decasteljau(part, t));
            
            curve.push_back(part.back());
            curves.push_back(curve);
        }
        return curves;
    }
    
    vector<Point> elevate_degree(const vector<Point>& c_points) {
        vector<Point> newpts;
        
        vector<vector<Point>> parts = divvy_points(c_points);
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
                    printf("overlap\n");
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
    static bool straight_enough(const vector<Point>& line) {
        unsigned long interval = line.size()/6;
        if (interval < 1)
            interval = 1;
        
        Point axis = Point(1,0), vector = line[interval] - line[0];
        float a1 = vector.angle(axis), a2;
        for (unsigned long p = interval; p + interval < line.size(); p += interval) {
            vector = line[p + interval] - line[p];
            a2 = vector.angle(axis);
            
            if (fabs(a1 - a2) > ANGLE_TOLERANCE)
                return false;
            
            a1 = a2;
        }
        printf("Stright line size: %lu\n", line.size());
        return true;
    }
    
    static vector<Point> straight_intersect(const Point p0, const Point p1, const Point p2, const Point p3) {
        vector<Point> r;
        
        Point s1, s2;
        s1 = p1 - p0;
        s2 = p3 - p2;
        
        float s, t;
        s = (-s1.y * (p0.x - p2.x) + s1.x * (p0.y - p2.y)) / (-s2.x * s1.y + s1.x * s2.y);
        t = ( s2.x * (p0.y - p2.y) - s2.y * (p0.x - p2.x)) / (-s2.x * s1.y + s1.x * s2.y);
        
        if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
            r.push_back(Point(p0.x + (t * s1.x), p0.y + (t * s1.y)));
        
        return r;
    }
    
    static vector<Point> intersect_search(const vector<Point>& A, const vector<Point>& B) {
        vector<Point> intersections;
        if(straight_enough(A) && straight_enough(B)){
            return straight_intersect(A.front(), A.back(), B.front(), B.back());
        }
        
        auto A1 = vector<Point>(A.begin(), A.begin() + (A.size() / 2));
        auto A2 = vector<Point>(A.begin() + (A.size() / 2), A.end());
        auto B1 = vector<Point>(B.begin(), B.begin() + (B.size() / 2));
        auto B2 = vector<Point>(B.begin() + (B.size() / 2), B.end());
        
        vector<Point> A1B1, A1B2, A2B1, A2B2;
        if (overlap(bounding_box(A1), bounding_box(B1)))
            A1B1 = intersect_search(A1, B1);
        intersections.insert(intersections.end(), A1B1.begin(), A1B1.end());
        if (overlap(bounding_box(A1), bounding_box(B2)))
            A1B2 = intersect_search(A1, B2);
        intersections.insert(intersections.end(), A1B2.begin(), A1B2.end());
        if (overlap(bounding_box(A2), bounding_box(B1)))
            A2B1 = intersect_search(A2, B1);
        intersections.insert(intersections.end(), A2B1.begin(), A2B1.end());
        if (overlap(bounding_box(A2), bounding_box(B2)))
            A2B2 = intersect_search(A2, B2);
        intersections.insert(intersections.end(), A2B2.begin(), A2B2.end());
        
        return intersections;
    }
    
    static bool overlap(const pair<Point, Point>& crvA, const pair<Point, Point>& crvB) {
        if (crvA.first.x < crvB.second.x && crvA.second.x > crvB.first.x &&
            crvA.first.y < crvB.second.y && crvA.second.y > crvB.first.y)
            return true;
        return false;
    }
    
    static pair<Point, Point> bounding_box(const vector<Point>& curve) {
        float minx = curve[0].x, maxx = curve[0].x, miny = curve[0].y, maxy = curve[0].y;
        for (auto &pt : curve) {
            if (pt.x < minx) minx = pt.x;
            if (pt.x > maxx) maxx = pt.x;
            if (pt.y < miny) miny = pt.y;
            if (pt.y > maxy) maxy = pt.y;
        }
        return pair<Point, Point>(Point(minx, miny), Point(maxx, maxy));
    }
};

#endif /* bezier_h */

#ifndef curve_h
#define curve_h

#include <vector>
#include <utility>

using namespace std;

class curve {
public:
    virtual vector<vector<Point>>& generate(const vector<Point> pts, const int degree)=0;
    virtual vector<Point> find_intersections(void) { return vector<Point>(); }

protected:
    vector<vector<Point>> curves;
    
    //TODO: add shared interpolation and bspline stuff
};

class lagrange : public curve {
public:
    lagrange() { curves = vector<vector<Point>>(); }
    
    vector<vector<Point>>& generate(const vector<Point> pts, const int degree) {
        //TODO: all of this
        return curves;
    }
};

class bezier : public curve {
public:
    bezier() { curves = vector<vector<Point>>(); }
    
    vector<vector<Point>>& generate(const vector<Point> pts, const int degree) {
        //TODO: all of this
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

class bspline : public curve {
public:
    bspline() { curves = vector<vector<Point>>(); }
    
    vector<vector<Point>>& generate(const vector<Point> pts, const int degree) {
        //TODO: all of this
        return curves;
    }
};

class catmullrom : public curve {
public:
    catmullrom() { curves = vector<vector<Point>>(); }
    
    vector<vector<Point>>& generate(const vector<Point> pts, const int degree) {
        //TODO: all of this
        return curves;
    }
};

#endif /* curve_h */

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <GLUT/GLUT.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "lagrange.h"
#include "bezier.h"
#include "bspline.h"
#include "catmullrom.h"
#include <vector>
#include <list>

int mouseX = -1, mouseY = -1;
bool mouseLeftDown = false, mouseRightDown = false, mouseMiddleDown = false;

bool drawpoly = false, selected_point = false;
unsigned int point_selection;
curve *selected_curve, *moving;

using namespace std;

vector<Point> new_points;
list<curve*> curves;

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    // draw the curves
    for (auto &crv : curves){
        vector<Point> control_points = crv->get_c_points();
        if (drawpoly) {
            glColor3f(0.8, 0.8, 0.8);
            glBegin(GL_LINE_STRIP);
            for (auto &p : control_points)
                glVertex2d(p.x, p.y);
            glEnd();
        }
        
        vector<Point> curve = crv->get_curve();
        
        if (crv == selected_curve)
            glColor3f(0.5, 0.0, 0.0);
        else
            glColor3f(0.0, 0.0, 0.0);
        
        glBegin(GL_LINE_STRIP);
        for (auto &pt : curve)
            glVertex2d(pt.x, pt.y);
        glEnd();
        
        for(auto &pt : control_points)
            pt.draw(0, 0, 1);
        
        if (crv == selected_curve && selected_point) {
            control_points[point_selection].draw(1, 0, 0);
        }
    }
    
    // draw the control points
    for(auto &knot : new_points)
        knot.draw(0, 0, 1);
    
    glFlush();
    glutSwapBuffers();
}

void init(void) {
    /* select clearing color 	*/
    glClearColor(1.0, 1.0, 1.0, 0.0);
    
    glDisable(GL_DEPTH_TEST);
    
    glShadeModel(GL_SMOOTH);
    
    glDisable(GL_CULL_FACE);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH) - 1, glutGet(GLUT_WINDOW_HEIGHT) - 1, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int button, int state, int x, int y) {
    switch(button)
    {
        case GLUT_LEFT_BUTTON:
            mouseLeftDown = state == GLUT_DOWN;
            selected_point = false;
            moving = nullptr;
            if(mouseLeftDown) {
                for (int p = 0; p < new_points.size(); p++) {
                    if (new_points[p].clicked(x, y)) {
                        point_selection = p;
                        selected_point = true;
                    }
                }
                if (!selected_point) {
                    for (auto &crv : curves) {
                        vector<Point> c_points = crv->get_c_points();
                        for (int p = 0; p < c_points.size(); p++) {
                            if (c_points[p].clicked(x, y)) {
                                point_selection = p;
                                selected_point = true;
                                moving = crv;
                            }
                        }
                    }
                }
                
                if (!selected_point)
                    new_points.push_back(Point(x, y));
                
            }
            glutPostRedisplay();
            break;
        case GLUT_MIDDLE_BUTTON:
            mouseMiddleDown = state == GLUT_DOWN;
            break;
        case GLUT_RIGHT_BUTTON:
            mouseRightDown = state == GLUT_DOWN;
            if (mouseRightDown) {
                selected_curve = nullptr;
                for (auto &crv : curves) {
                    vector<Point> c_points = crv->get_c_points();
                    for (auto &pt : c_points) {
                        if (pt.clicked(x, y))
                            selected_curve = crv;
                    }
                }
            }
            
            glutPostRedisplay();
            break;
    }
    mouseX = x;
    mouseY = y;
}

void motion(int x, int y) {
    float dx, dy;
    dx = x - mouseX;
    dy = y - mouseY;
    
    if (selected_point) {
        if (moving)
            moving->move_point(point_selection, dx, dy);
        else
            new_points[point_selection].diff(dx, dy);
        
        glutPostRedisplay();
    }
    
    mouseX = x;
    mouseY = y;
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH) - 1, glutGet(GLUT_WINDOW_HEIGHT) - 1, 0);
    glMatrixMode(GL_MODELVIEW);
}

void save_file() {
    string filename;
    printf("save file: ");
    cin >> filename;
    ofstream file;
    file.open(filename);
    file << curves.size() << endl;
    for (auto &crv : curves) {
        file << crv->get_type() << endl;
        file << crv->get_degree() << ' ' << crv->get_fidelity() << ' ' \
            << crv->get_param() << ' ' << crv->get_c_points().size() << endl;
        for (auto & pt : crv->get_c_points())
            file << pt.x << ' ' << pt.y << endl;
    }
    file.close();
}

void load_file() {
    string filename;
    printf("load file: ");
    cin >> filename;
    ifstream file;
    file.open(filename);
    curves.clear();
    unsigned int num_curves;
    file >> num_curves;
    for (unsigned int i = 0; i < num_curves; i++) {
        int type;
        file >> type;
        
        unsigned int degree, size;
        float fidel, param;
        file >> degree >> fidel >> param >> size;
        
        curve *crv;
        switch (type) {
            case curve::lagrange:
                crv = new lagrange(degree, fidel, param);
                break;
            case curve::bezier:
                crv = new bezier(degree, fidel, param);
                break;
            case curve::bspline:
                crv = new bspline(degree, fidel, param);
                break;
            case curve::catmullrom:
                crv = new catmullrom(degree, fidel, param);
                break;
                
            default:
                printf("file error: invalid type\n");
                break;
        }
        
        vector<Point> c_points;
        for (int j = 0; j < size; j++) {
            float x,y;
            file >> x >> y;
            c_points.push_back(Point(x, y));
        }
        crv->generate(c_points);
        curves.push_back(crv);
    }
    file.close();
}

void key(unsigned char c, int x, int y) {
    switch (c) {
        case '1':
            if (new_points.size() > 1) {
                curves.push_back(new lagrange(new_points.size() - 1));
                curves.back()->generate(new_points);
                new_points.clear();
                
                printf("LaGrange curve, degree %lu\n", new_points.size() - 1);
                glutPostRedisplay();
            }
            break;
            
        case '2':
            if (new_points.size() > 1) {
                curves.push_back(new bezier(new_points.size() - 1));
                curves.back()->generate(new_points);
                new_points.clear();
                
                printf("Bezier curve, degree %lu\n", new_points.size() - 1);
                glutPostRedisplay();
            }
            break;
            
        case '3':
            if (new_points.size() > 1) {
                curves.push_back(new bspline(1));
                curves.back()->generate(new_points);
                new_points.clear();
                
                printf("B-spline curve, degree %lu\n", new_points.size() - 1);
                glutPostRedisplay();
            }
            break;
            
        case '4':
            if (new_points.size() > 1) {
                curves.push_back(new catmullrom(1));
                curves.back()->generate(new_points);
                new_points.clear();
                
                printf("Catmull-Rom curve, degree %lu\n", new_points.size() - 1);
                glutPostRedisplay();
            }
            break;
            
        case '7':
            if (selected_curve) {
                selected_curve->set_uniform_param();
                printf("Set uniform parameterization, a = 0.0\n");
                selected_curve->generate();
                glutPostRedisplay();
            }
            break;
            
        case '8':
            if (selected_curve) {
                selected_curve->set_centrip_param();
                printf("Set centripital parameterization, a = 0.5\n");
                selected_curve->generate();
                glutPostRedisplay();
            }
            break;
            
        case '9':
            if (selected_curve) {
                selected_curve->set_chordlen_param();
                printf("Set chord length parameterization, a = 1.0\n");
                selected_curve->generate();
                glutPostRedisplay();
            }
            break;
            
        case '0':
            if (selected_curve) {
                printf("Set unusual parameterization, a = ");
                float param;
                cin >> param;
                selected_curve->set_unusual_param(param);
                selected_curve->generate();
                glutPostRedisplay();
            }
            break;
            
        case '=':
            if (selected_curve) {
                if (selected_curve->get_type() > curve::bezier) {
                    selected_curve->degree_inc();
                    selected_curve->generate();
                    printf("increase degree to %u\n", selected_curve->get_degree());
                    glutPostRedisplay();
                }
            }
            break;
            
        case '-':
            if (selected_curve) {
                if (selected_curve->get_type() > curve::bezier) {
                    selected_curve->degree_dec();
                    selected_curve->generate();
                    printf("decrease degree to %u\n", selected_curve->get_degree());
                    glutPostRedisplay();
                }
            }
            break;
            
        case 'o':
            if (curves.size())
                save_file();
            break;
            
        case 'l':
            load_file();
            glutPostRedisplay();
            break;
            
        case '\\':
            if (selected_curve) {
                auto it = curves.begin();
                for (; *it != selected_curve && it != curves.end(); it++);
                if (it == curves.end())
                    printf("Panic");
                curves.erase(it);
                delete selected_curve;
                selected_curve = nullptr;
            }
            else if (new_points.size())
                new_points.pop_back();
            glutPostRedisplay();
            break;
            
        case '[':
            if (selected_curve && selected_curve->get_type() == curve::bezier) {
                selected_curve->elevate_degree();
                selected_curve->generate();
                printf("elevated degree of curve(s) to %u\n", selected_curve->get_degree());
                glutPostRedisplay();
            }
            break;
            
        case 'p':
            drawpoly = !drawpoly;
            
            if (drawpoly)
                printf("draw polylines\n");
            else
                printf("hide polylines\n");
            
            glutPostRedisplay();
            break;
            
        default:
            //printf("unmapped key %c hit\n", c);
            break;
    }
}

int main(int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("CPSC 645 HW1 - Matthew Dillard");
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(key);
    glutMainLoop();
    return 0;
}

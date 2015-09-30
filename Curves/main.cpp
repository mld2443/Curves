#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <GLUT/GLUT.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "lagrange.h"
#include "bezier.h"
#include "bspline.h"
#include "catmullrom.h"
#include <vector>

int mouseX = -1, mouseY = -1;
bool mouseLeftDown = false, mouseRightDown = false, mouseMiddleDown = false;

bool editmode = true, drawcurve = false, drawpoly = false, grouppoly = true, intersect = false;
Point* movepoint;

using namespace std;

vector<Point> c_points;
vector<Point> intersections;
vector<vector<Point>> crvs;
curve* cgen;

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    // draw the curves
    if (drawcurve) {
        // draw the polylines for control "polygon"
        if (drawpoly) {
            glColor3f(0.8, 0.8, 0.8);
            if (grouppoly) {
                for (int i = 0; i + cgen->get_degree() < c_points.size(); i += cgen->get_degree() + 1){
                    glBegin(GL_LINE_STRIP);
                    for(int j = i; j <= i + cgen->get_degree(); j++)
                        glVertex2d(c_points[j].x, c_points[j].y);
                    glEnd();
                }
            }
            else {
                glBegin(GL_LINE_STRIP);
                for (auto &p : c_points)
                    glVertex2d(p.x, p.y);
                glEnd();
            }
        }
        
        // the actual curve
        crvs = cgen->generate(c_points);
        glColor3f(0.0, 0.0, 0.0);
        for (auto &crv : crvs) {
            glBegin(GL_LINE_STRIP);
            for (auto &pt : crv)
                glVertex2d(pt.x, pt.y);
            glEnd();
        }
    }
    
    // draw intersection points
    if (intersect) {
        for (auto &pt : intersections)
            pt.draw(1, 0, 0);
        intersections.clear();
        intersect = false;
    }
    
    // draw the control points
    for(auto &knot : c_points)
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
    
    movepoint = nullptr;
}

void mouse(int button, int state, int x, int y) {
    switch(button)
    {
        case GLUT_LEFT_BUTTON:
            mouseLeftDown = state == GLUT_DOWN;
            if(editmode && mouseLeftDown) {
                c_points.push_back(Point(x, y));
                glutPostRedisplay();
            }
            else if (mouseLeftDown) {
                for (auto &p : c_points) {
                    if (p.clicked(x, y))
                        movepoint = &(p);
                }
            }
            else {
                movepoint = nullptr;
                glutPostRedisplay();
            }
            break;
        case GLUT_MIDDLE_BUTTON:
            mouseMiddleDown = state == GLUT_DOWN;
            break;
        case GLUT_RIGHT_BUTTON:
            mouseRightDown = state == GLUT_DOWN;
            break;
    }
    mouseX = x;
    mouseY = y;
}

void motion(int x, int y) {
    float dx, dy;
    dx = x - mouseX;
    dy = y - mouseY;
    
    if (!editmode && movepoint != nullptr) {
        movepoint->diff(dx, dy);
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

void key(unsigned char c, int x, int y) {
    switch (c) {
        case 'e':
            editmode = false;
            drawcurve = false;
            printf("display mode\n");
            break;
            
        case '1':
            if (!editmode) {
                drawcurve = true;
                grouppoly = true;
                
                if (cgen != nullptr)
                    free(cgen);
                cgen = new lagrange();
                
                printf("LaGrange curve, degree %u\n", cgen->get_degree());
                glutPostRedisplay();
            }
            break;
            
        case '2':
            if (!editmode) {
                drawcurve = true;
                grouppoly = true;
                
                if (cgen != nullptr)
                    free(cgen);
                cgen = new bezier();
                
                printf("Bezier curve, degree %u\n", cgen->get_degree());
                glutPostRedisplay();
            }
            break;
            
        case '3':
            if (!editmode) {
                drawcurve = true;
                grouppoly = false;
                
                if (cgen != nullptr)
                    free(cgen);
                cgen = new bspline();
                
                printf("B-spline curve, degree %u\n", cgen->get_degree());
                glutPostRedisplay();
           }
            break;
            
        case '4':
            if (!editmode) {
                drawcurve = true;
                grouppoly = false;
                
                if (cgen != nullptr)
                    free(cgen);
                cgen = new catmullrom();
                
                printf("Catmull-Rom curve, smoothness %u\n", cgen->get_degree());
                //glutPostRedisplay();
            }
            break;
            
        case '7':
            if (drawcurve) {
                cgen->set_uniform_param();
                printf("Set uniform parameterization, a = 0.0\n");
                glutPostRedisplay();
            }
            break;
            
        case '8':
            if (drawcurve) {
                cgen->set_centrip_param();
                printf("Set centripital parameterization, a = 0.5\n");
                glutPostRedisplay();
            }
            break;
            
        case '9':
            if (drawcurve) {
                cgen->set_chordlen_param();
                printf("Set chord length parameterization, a = 1.0\n");
                glutPostRedisplay();
            }
            break;
            
        case '0':
            if (drawcurve) {
                printf("Set unusual parameterization, a = ");
                float param;
                cin >> param;
                cgen->set_unusual_param(param);
                glutPostRedisplay();
            }
            break;
            
        case '=':
            if (drawcurve) {
                cgen->degree_inc();
                printf("increase degree to %u\n", cgen->get_degree());
                glutPostRedisplay();
           }
            break;
            
        case '-':
            if (drawcurve) {
                cgen->degree_dec();
                printf("decrease degree to %u\n", cgen->get_degree());
                glutPostRedisplay();
            }
            break;
            
        case 'i':
            if (drawcurve) {
                intersections = cgen->find_intersections();
                intersect = true;
                printf("calculated intersections: %lu\n", intersections.size());
                glutPostRedisplay();
            }
            break;
            
        case 'o':
            if (drawcurve) {
                c_points = cgen->elevate_degree(c_points);
                printf("elevated degree of curve(s) to %u\n", cgen->get_degree());
                glutPostRedisplay();
            }
            break;
            
        case 'p':
            if (drawcurve) {
                drawpoly = !drawpoly;
                
                if (drawpoly)
                    printf("draw polylines\n");
                else
                    printf("hide polylines\n");
                
                glutPostRedisplay();
            }
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

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <GLUT/GLUT.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Point.h"
#include "curve.h"
#include <vector>

int mouseX = -1, mouseY = -1;
bool mouseLeftDown = false, mouseRightDown = false, mouseMiddleDown = false;

bool editmode = true, drawcurve = false;
Point* movepoint;

curve* cgen;
int degree = 1;

using namespace std;

vector<Point> pts;
vector<vector<Point>> crvs;


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    // draw the curves
    if (drawcurve) {
        for (vector<vector<Point>>::iterator crv = crvs.begin(); crv != crvs.end(); crv++) {
            glBegin(GL_LINE_STRIP);
            for (vector<Point>::iterator pt = crv->begin(); pt != crv->end(); pt++)
                glVertex2d(pt->x, pt->y);
            glEnd();
        }
    }
    
    // draw the control points
    for(vector<Point>::iterator it = pts.begin(); it != pts.end(); it++)
        it->draw(0, 0, 1);
    
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
                pts.push_back(Point(x, y));
                glutPostRedisplay();
            }
            else if (mouseLeftDown) {
                for (vector<Point>::iterator it = pts.begin(); it != pts.end(); it++) {
                    if (it->clicked(x, y))
                        movepoint = &(*it);
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
                cgen = new lagrange();
                printf("LaGrange curve, degree %u\n", degree);
            }
            break;
            
        case '2':
            if (!editmode) {
                drawcurve = true;
                cgen = new bezier();
                printf("Bezier curve, degree %u\n", degree);
            }
            break;
            
        case '3':
            if (!editmode) {
                drawcurve = true;
                cgen = new bspline();
                printf("B-spline curve, degree %u\n", degree);
           }
            break;
            
        case '4':
            if (!editmode) {
                drawcurve = true;
                cgen = new catmullrom();
                degree = 0;
                printf("Catmull-Rom curve, smoothness %u\n", degree);
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

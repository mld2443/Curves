#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <GLUT/GLUT.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Point.h"
#include <vector>

int mouseX = -1, mouseY = -1;
bool mouseLeftDown = false, mouseRightDown = false, mouseMiddleDown = false;

using namespace std;

vector<Point> pts;


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    for ( int i = 0; i < pts.size ( ); i++ )
    {
        pts [ i ].draw ( );
    }
    
    glFlush ( );
    glutSwapBuffers();
}

void init(void)
{
    /* select clearing color 	*/
    glClearColor ( 1.0, 1.0, 1.0, 0.0 );
    
    glDisable ( GL_DEPTH_TEST );
    
    glShadeModel ( GL_SMOOTH );
    
    glDisable ( GL_CULL_FACE );
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity ( );
    gluOrtho2D ( 0, glutGet ( GLUT_WINDOW_WIDTH ) - 1, glutGet ( GLUT_WINDOW_HEIGHT ) - 1, 0 );
    
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity ( );
}

void mouse(int button, int state, int x, int y)
{
    switch ( button )
    {
        case GLUT_LEFT_BUTTON:
            mouseLeftDown = state == GLUT_DOWN;
            if ( mouseLeftDown )
            {
                pts.push_back ( Point ( x, y ) );
                glutPostRedisplay ( );
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

void motion(int x, int y)
{
    float dx, dy;
    dx = ( x - mouseX );
    dy = ( y - mouseY );
    mouseX = x;
    mouseY = y;
    
    glutPostRedisplay ( );
}

void reshape ( int w, int h )
{
    glViewport ( 0, 0, w, h );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity ( );
    gluOrtho2D ( 0, glutGet ( GLUT_WINDOW_WIDTH ) - 1, glutGet ( GLUT_WINDOW_HEIGHT ) - 1, 0 );
    glMatrixMode ( GL_MODELVIEW );
}

void key ( unsigned char c, int x, int y )
{
    printf ( "%c hit\n", c );
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("CPSC 645 HW1 - <Your Name Here>");
    init();
    glutReshapeFunc ( reshape );
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(key);
    glutMainLoop();
    return 0;
}
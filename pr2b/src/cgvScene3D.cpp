#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#else

#include <GL/glut.h>

#endif

#include <cstdlib>
#include <stdio.h>

#include "cgvScene3D.h"


cgvScene3D::cgvScene3D() { axis = true; }

cgvScene3D::~cgvScene3D() {}

void paint_axes(void) {
    GLfloat red[] = { 1,0,0,1.0 };
    GLfloat green[] = { 0,1,0,1.0 };
    GLfloat blue[] = { 0,0,1,1.0 };

    glMaterialfv(GL_FRONT, GL_EMISSION, red);
    glBegin(GL_LINES);
    glVertex3f(1000, 0, 0);
    glVertex3f(-1000, 0, 0);
    glEnd();

    glMaterialfv(GL_FRONT, GL_EMISSION, green);
    glBegin(GL_LINES);
    glVertex3f(0, 1000, 0);
    glVertex3f(0, -1000, 0);
    glEnd();

    glMaterialfv(GL_FRONT, GL_EMISSION, blue);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 1000);
    glVertex3f(0, 0, -1000);
    glEnd();
}

void paint_tube() {
    GLUquadricObj *pipe;
    GLfloat tube_color[] = { 0,0,0.5 };

    glMaterialfv(GL_FRONT, GL_EMISSION, tube_color);

    pipe = gluNewQuadric();
    gluQuadricDrawStyle(pipe, GLU_FILL);

    glPushMatrix();
    glTranslatef(0, 0, -0.5);
    gluCylinder(pipe, 0.25, 0.25, 1, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(pipe);
}

void cgvScene3D::display(void) {
    // create lights
    GLfloat light0[] = { 10, 8, 9, 1 }; // point light
    glLightfv(GL_LIGHT0, GL_POSITION, light0);
    glEnable(GL_LIGHT0);

    // create the model
    glPushMatrix(); // save the modeling matrix

    // paint the axes
    if (axis) paint_axes();

    // paint the scene objects
    GLfloat cube_color[] = { 0, 0.25, 0 };
    glMaterialfv(GL_FRONT, GL_EMISSION, cube_color);

    glPushMatrix();
    glScalef(1, 2, 4);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glRotatef(45, 1, 0, 0);
    glScalef(1, 1, 4.5);
    paint_tube();
    glPopMatrix();

    glPushMatrix();
    glRotatef(-45, 1, 0, 0);
    glScalef(1, 1, 4.5);
    paint_tube();
    glPopMatrix();

    glPopMatrix(); // restores the modeling matrix
}


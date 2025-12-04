#include "ArticulatedModel.h"
#include <cmath>

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

// Helper to create a cylinder
static void draw_cylinder(float radius, float height, int slices) {
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, radius, radius, height, slices, 1);
    gluDeleteQuadric(quad);
}

ArticulatedModel::ArticulatedModel() : Object3D() {
    active_dof = 0;
    dof[0] = 0.0f; dof[1] = 0.0f; dof[2] = 0.0f;
    dof_min[0] = -180.0f; dof_max[0] = 180.0f;
    dof_min[1] = -90.0f;  dof_max[1] = 90.0f;
    dof_min[2] = -90.0f;  dof_max[2] = 90.0f;
}

void ArticulatedModel::draw() {
    glPushMatrix();
    applyTransformations();

    // Base
    glPushMatrix();
    glRotatef(dof[0], 0, 1, 0);
    glColor3f(0.4f, 0.4f, 0.5f);
    draw_base();

        // Joint 1 & Arm 1
        glPushMatrix();
        glTranslatef(0, 0.5, 0);
        glColor3f(0.8f, 0.2f, 0.2f);
        glutSolidSphere(0.4, 20, 20);
        glRotatef(dof[1], 1, 0, 0);
        draw_arm1();

            // Joint 2 & Arm 2
            glPushMatrix();
            glTranslatef(0, 2.0, 0);
            glColor3f(0.2f, 0.8f, 0.2f);
            glutSolidSphere(0.3, 20, 20);
            glRotatef(dof[2], 1, 0, 0);
            draw_arm2();

                // Head
                glPushMatrix();
                glTranslatef(0, 2.0, 0);
                glColor3f(0.2f, 0.2f, 0.8f);
                draw_head();
                glPopMatrix(); // Pop head

            glPopMatrix(); // Pop arm 2
        glPopMatrix(); // Pop arm 1
    glPopMatrix(); // Pop base

    glPopMatrix(); // Pop global transformations
}

void ArticulatedModel::render_for_selection() {
    glPushMatrix();
    applyTransformations();

    // Base (ID 1 -> DoF 0)
    glPushMatrix();
    glRotatef(dof[0], 0, 1, 0);
    glColor3ub(1, 0, 0);
    draw_base();

        // Arm 1 (ID 2 -> DoF 1)
        glPushMatrix();
        glTranslatef(0, 0.5, 0);
        glColor3ub(2, 0, 0);
        glutSolidSphere(0.4, 20, 20);
        glRotatef(dof[1], 1, 0, 0);
        draw_arm1();

            // Arm 2 (ID 3 -> DoF 2)
            glPushMatrix();
            glTranslatef(0, 2.0, 0);
            glColor3ub(3, 0, 0);
            glutSolidSphere(0.3, 20, 20);
            glRotatef(dof[2], 1, 0, 0);
            draw_arm2();
            glPopMatrix(); // Pop arm 2
        glPopMatrix(); // Pop arm 1
    glPopMatrix(); // Pop base

    glPopMatrix(); // Pop global
}

// Drawing methods no longer set their own color
void ArticulatedModel::draw_base() {
    glPushMatrix();
    glScalef(1.5, 1.0, 1.5);
    glutSolidCube(1.0);
    glPopMatrix();
}

void ArticulatedModel::draw_arm1() {
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    draw_cylinder(0.25, 2.0, 20);
    glPopMatrix();
}

void ArticulatedModel::draw_arm2() {
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    draw_cylinder(0.2, 2.0, 20);
    glPopMatrix();
}

void ArticulatedModel::draw_head() {
    glPushMatrix();
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
}

void ArticulatedModel::next_dof() { active_dof = (active_dof + 1) % 3; }
void ArticulatedModel::prev_dof() { active_dof = (active_dof - 1 + 3) % 3; }

void ArticulatedModel::increase_dof() {
    dof[active_dof] += 2.0f;
    if (dof[active_dof] > dof_max[active_dof]) dof[active_dof] = dof_max[active_dof];
}

void ArticulatedModel::decrease_dof() {
    dof[active_dof] -= 2.0f;
    if (dof[active_dof] < dof_min[active_dof]) dof[active_dof] = dof_min[active_dof];
}

void ArticulatedModel::set_dof(int dof_id) {
    if (dof_id >= 0 && dof_id < 3) active_dof = dof_id;
}

void ArticulatedModel::update(float time) {
    dof[0] = (dof_max[0] - dof_min[0]) * 0.5f * sin(time * 0.5f);
    dof[1] = dof_min[1] + (dof_max[1] - dof_min[1]) * 0.5f * (1 + sin(time * 0.7f));
    dof[2] = dof_min[2] + (dof_max[2] - dof_min[2]) * 0.5f * (1 + sin(time * 0.9f));
}

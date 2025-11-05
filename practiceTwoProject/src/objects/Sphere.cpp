//
// Created by Błażej Drozd on 27/09/2025.
//

#include "Sphere.h"

void Sphere::draw() {
    applyTransformations();
    if (getSelected()) glColor3f(0.0f, 1.0f, 0.0f);
    else glColor3f(0.5f, 0.5f, 0.5f);
    glutSolidSphere(0.3f, 20, 20);
    glPopMatrix();
}

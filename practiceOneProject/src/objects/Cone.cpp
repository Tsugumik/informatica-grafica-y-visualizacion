//
// Created by Błażej Drozd on 27/09/2025.
//

#include "Cone.h"

void Cone::draw() {
    applyTransformations();
    if (getSelected()) glColor3f(0.0f, 0.0f, 1.0f);
    else glColor3f(0.5f, 0.5f, 0.5f);
    glutSolidCone(0.3f, 0.6f, 20, 20);
    glPopMatrix();
}

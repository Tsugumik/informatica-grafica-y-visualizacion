//
// Created by Błażej Drozd on 27/09/2025.
//

#include "Cube.h"

void Cube::draw() {
    applyTransformations();
    if (getSelected()) glColor3f(1.0f, 0.0f, 0.0f);
    else glColor3f(0.5f, 0.5f, 0.5f);
    glutSolidCube(0.5f);
    glPopMatrix();
}

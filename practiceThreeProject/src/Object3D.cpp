//
// Created by Błażej Drozd on 27/09/2025.
//

#include "Object3D.h"

Object3D::Object3D() {
    translateX = translateY = translateZ = 0.0f;
    rotateX = rotateY = rotateZ = 0.0f;
    scaleX = scaleY = scaleZ = 1.0f;
    isSelected = false;
    rstMode = true;
    transformationHistory.clear();
}

Object3D::~Object3D() {
    // Destructor
}

void Object3D::translate(float dx, float dy, float dz) {
    translateX += dx;
    translateY += dy;
    translateZ += dz;

    if (!rstMode) {
        transformationHistory.emplace_back(TRANSLATE_OP, dx, dy, dz);
    }

    std::cout << "Translation: (" << translateX << ", " << translateY << ", " << translateZ << ")" << std::endl;
}

void Object3D::rotate(float rx, float ry, float rz) {
    rotateX += rx;
    rotateY += ry;
    rotateZ += rz;

    if (!rstMode) {
        transformationHistory.emplace_back(ROTATE_OP, rx, ry, rz);
    }

    std::cout << "Rotation: (" << rotateX << ", " << rotateY << ", " << rotateZ << ")" << std::endl;
}

void Object3D::scale(float sx, float sy, float sz) {
    scaleX *= sx;
    scaleY *= sy;
    scaleZ *= sz;

    if (!rstMode) {
        transformationHistory.emplace_back(SCALE_OP, sx, sy, sz);
    }

    std::cout << "Scale: (" << scaleX << ", " << scaleY << ", " << scaleZ << ")" << std::endl;
}

void Object3D::resetTransformations() {
    translateX = translateY = translateZ = 0.0f;
    rotateX = rotateY = rotateZ = 0.0f;
    scaleX = scaleY = scaleZ = 1.0f;
}

void Object3D::applyTransformations() {
    // Removed glPushMatrix() and glPopMatrix() from here
    if (rstMode) {
        // RST Mode: Always T-S-R order (reverse of desired R-S-T)
        glTranslatef(translateX, translateY, translateZ);
        glScalef(scaleX, scaleY, scaleZ);
        glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
        glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);
    } else {
        // Sequential Mode: Apply in order of pressing
        for (const auto& step : transformationHistory) {
            switch (step.type) {
                case TRANSLATE_OP:
                    glTranslatef(step.x, step.y, step.z);
                    break;
                case ROTATE_OP:
                    if (step.x != 0.0f) glRotatef(step.x, 1.0f, 0.0f, 0.0f);
                    if (step.y != 0.0f) glRotatef(step.y, 0.0f, 1.0f, 0.0f);
                    if (step.z != 0.0f) glRotatef(step.z, 0.0f, 0.0f, 1.0f);
                    break;
                case SCALE_OP:
                    glScalef(step.x, step.y, step.z);
                    break;
            }
        }
    }
}

void Object3D::clearTransformationHistory() {
    transformationHistory.clear();
}

//
// Created by Błażej Drozd on 27/09/2025.
//

#ifndef PR1_OBJECT3D_H
#define PR1_OBJECT3D_H

#include <iostream>
#include <vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

enum TransformationType {
    TRANSLATE_OP,
    ROTATE_OP,
    SCALE_OP
};

struct TransformationStep {
    TransformationType type;
    float x, y, z;

    TransformationStep(TransformationType t, float _x, float _y, float _z)
        : type(t), x(_x), y(_y), z(_z) {}
};

class Object3D {
private:
    float translateX, translateY, translateZ;
    float rotateX, rotateY, rotateZ;
    float scaleX, scaleY, scaleZ;
    bool isSelected;

    bool rstMode; // true = RST, false = Sequential

    std::vector<TransformationStep> transformationHistory;

public:
    Object3D();

    virtual ~Object3D();

    // Transformation methods
    void translate(float dx, float dy, float dz);

    void rotate(float rx, float ry, float rz);

    void scale(float sx, float sy, float sz);

    void resetTransformations();

    // Selection
    void setSelected(bool selected) { isSelected = selected; }
    bool getSelected() const { return isSelected; }

    void setRSTMode(bool mode) { rstMode = mode; }

    // Virtual methods to be overridden by child classes
    virtual void draw() = 0;

    // Apply transformations
    void applyTransformations();

    void clearTransformationHistory();
};


#endif //PR1_OBJECT3D_H

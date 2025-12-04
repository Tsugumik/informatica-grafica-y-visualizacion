#ifndef PRACTICE_THREE_PROJECT_QUAD_H
#define PRACTICE_THREE_PROJECT_QUAD_H

#include "cgvTriangleMesh.h"

class Quad : public cgvTriangleMesh {
public:
    Quad(float width = 20.0f, float depth = 20.0f);
    ~Quad() override = default;
};

#endif //PRACTICE_THREE_PROJECT_QUAD_H

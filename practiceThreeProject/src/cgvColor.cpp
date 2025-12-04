#include "cgvColor.h"

cgvColor::cgvColor(const float r, const float g, const float b) {
    color[R] = r;
    color[G] = g;
    color[B] = b;
    color[A] = 1.0f; // Default alpha to opaque
}

cgvColor::cgvColor(const float r, const float g, const float b, const float a) {
    color[R] = r;
    color[G] = g;
    color[B] = b;
    color[A] = a;
}

cgvColor::cgvColor(const cgvColor& p) {
    color[R] = p.color[R];
    color[G] = p.color[G];
    color[B] = p.color[B];
    color[A] = p.color[A];
}

void cgvColor::apply() {
    glColor4fv(color);
}

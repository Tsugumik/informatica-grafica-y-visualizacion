#include "cgvPoint3D.h"

// Constructors
cgvPoint3D::cgvPoint3D(const float& x, const float& y, const float& z, const float& w) {
    c[X] = x;
    c[Y] = y;
    c[Z] = z;
    c[3] = w;
}

cgvPoint3D::cgvPoint3D(const cgvPoint3D& p) {
    c[X] = p.c[X];
    c[Y] = p.c[Y];
    c[Z] = p.c[Z];
    c[3] = p.c[3];
}

cgvPoint3D& cgvPoint3D::operator=(const cgvPoint3D& p) {
    if (this != &p) {
        c[X] = p.c[X];
        c[Y] = p.c[Y];
        c[Z] = p.c[Z];
        c[3] = p.c[3];
    }
    return *this;
}

// Set method
void cgvPoint3D::set(const float& x, const float& y, const float& z, const float& w) {
    c[X] = x;
    c[Y] = y;
    c[Z] = z;
    c[3] = w;
}

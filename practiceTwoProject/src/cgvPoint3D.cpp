#include "cgvPoint3D.h"

cgvPoint3D::cgvPoint3D(const GLfloat& x, const GLfloat& y, const GLfloat& z) {
    c[X] = x; c[Y] = y; c[Z] = z;
}

cgvPoint3D::cgvPoint3D(const cgvPoint3D& p) {
    c[X] = p.c[X]; c[Y] = p.c[Y]; c[Z] = p.c[Z];
}

cgvPoint3D& cgvPoint3D::operator=(const cgvPoint3D& p) {
    if (this != &p) {
        c[X] = p.c[X]; c[Y] = p.c[Y]; c[Z] = p.c[Z];
    }
    return *this;
}

cgvPoint3D cgvPoint3D::operator+(const cgvPoint3D& p) const {
    return cgvPoint3D(c[X] + p.c[X], c[Y] + p.c[Y], c[Z] + p.c[Z]);
}

cgvPoint3D& cgvPoint3D::operator+=(const cgvPoint3D& p) {
    c[X] += p.c[X]; c[Y] += p.c[Y]; c[Z] += p.c[Z];
    return *this;
}

cgvPoint3D cgvPoint3D::operator-(const cgvPoint3D& p) const {
    return cgvPoint3D(c[X] - p.c[X], c[Y] - p.c[Y], c[Z] - p.c[Z]);
}

cgvPoint3D cgvPoint3D::cross(const cgvPoint3D& p) const {
    return cgvPoint3D(
        c[Y] * p.c[Z] - c[Z] * p.c[Y],
        c[Z] * p.c[X] - c[X] * p.c[Z],
        c[X] * p.c[Y] - c[Y] * p.c[X]
    );
}

GLfloat cgvPoint3D::length() const {
    return sqrt(c[X] * c[X] + c[Y] * c[Y] + c[Z] * c[Z]);
}

void cgvPoint3D::normalize() {
    GLfloat len = length();
    if (len > IGV_EPSILON) {
        c[X] /= len;
        c[Y] /= len;
        c[Z] /= len;
    }
}

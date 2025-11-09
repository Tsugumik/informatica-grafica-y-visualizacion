#ifndef __IGVPUNTO3D
#define __IGVPUNTO3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>

#define IGV_EPSILON 0.000001

#ifndef __ENUM_XYZ
#define __ENUM_XYZ
enum Coordinates { X, Y, Z };
#endif

class cgvPoint3D {
private:
    GLfloat c[3] = {0,0,0};

public:
    cgvPoint3D() = default;
    cgvPoint3D(const GLfloat& x, const GLfloat& y, const GLfloat& z);
    cgvPoint3D(const cgvPoint3D& p);
    ~cgvPoint3D() = default;

    cgvPoint3D& operator=(const cgvPoint3D& p);
    GLfloat& operator[](unsigned char idx);
    GLfloat operator[](unsigned char idx) const;

    cgvPoint3D operator+(const cgvPoint3D& p) const;
    cgvPoint3D& operator+=(const cgvPoint3D& p);
    cgvPoint3D operator-(const cgvPoint3D& p) const;

    cgvPoint3D cross(const cgvPoint3D& p) const;
    void normalize();
    GLfloat length() const;
};

inline GLfloat& cgvPoint3D::operator[](unsigned char idx) { return c[idx]; }
inline GLfloat cgvPoint3D::operator[](unsigned char idx) const { return c[idx]; }

#endif

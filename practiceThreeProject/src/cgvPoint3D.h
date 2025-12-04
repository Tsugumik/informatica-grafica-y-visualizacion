#ifndef __CGVPOINT3D
#define __CGVPOINT3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include <cmath> // For sqrtf
#include <vector>

#define CGV_EPSILON 0.000001f ///< Constant for comparisons with 0

#ifndef __ENUM_XYZ
#define __ENUM_XYZ
enum Coordinates { X, Y, Z };
#endif // __ENUM_XYZ

class cgvPoint3D {
private:
    float c[4] = {0.0f, 0.0f, 0.0f, 1.0f};

public:
    // Constructors
    cgvPoint3D() = default;
    cgvPoint3D(const float& x, const float& y, const float& z, const float& w = 1.0f);

    // Copy constructor
    cgvPoint3D(const cgvPoint3D& p);

    // Assignment operator
    cgvPoint3D& operator=(const cgvPoint3D& p);

    ~cgvPoint3D() = default;

    // Access operators
    float& operator[](const unsigned char idx);
    const float& operator[](const unsigned char idx) const;

    // Comparison operators
    bool operator==(const cgvPoint3D& p) const;
    bool operator!=(const cgvPoint3D& p) const;

    // Set method
    void set(const float& x, const float& y, const float& z, const float& w = 1.0f);

    // Conversion
    const float* data() const;

    // Vector operations
    cgvPoint3D operator+(const cgvPoint3D& other) const;
    cgvPoint3D& operator+=(const cgvPoint3D& other);
    cgvPoint3D operator-(const cgvPoint3D& other) const;
    cgvPoint3D cross(const cgvPoint3D& other) const;
    void normalize();
    float length() const;
};

// Inline implementations

inline float& cgvPoint3D::operator[](const unsigned char idx) { return c[idx]; }
inline const float& cgvPoint3D::operator[](const unsigned char idx) const { return c[idx]; }

inline bool cgvPoint3D::operator==(const cgvPoint3D& p) const {
    return ( ( fabsf( c[X] - p[X] ) < CGV_EPSILON ) &&
             ( fabsf( c[Y] - p[Y] ) < CGV_EPSILON ) &&
             ( fabsf( c[Z] - p[Z] ) < CGV_EPSILON ) );
}

inline bool cgvPoint3D::operator!=(const cgvPoint3D& p) const {
    return !(*this == p);
}

inline cgvPoint3D cgvPoint3D::operator+(const cgvPoint3D& other) const {
    return {c[X] + other.c[X], c[Y] + other.c[Y], c[Z] + other.c[Z], c[3]};
}

inline cgvPoint3D& cgvPoint3D::operator+=(const cgvPoint3D& other) {
    c[X] += other.c[X];
    c[Y] += other.c[Y];
    c[Z] += other.c[Z];
    return *this;
}

inline cgvPoint3D cgvPoint3D::operator-(const cgvPoint3D& other) const {
    return {c[X] - other.c[X], c[Y] - other.c[Y], c[Z] - other.c[Z], c[3]};
}

inline cgvPoint3D cgvPoint3D::cross(const cgvPoint3D& other) const {
    return {
        c[Y] * other.c[Z] - c[Z] * other.c[Y],
        c[Z] * other.c[X] - c[X] * other.c[Z],
        c[X] * other.c[Y] - c[Y] * other.c[X],
        0.0f // The cross product of two vectors is a vector
    };
}

inline float cgvPoint3D::length() const {
    return sqrtf(c[X] * c[X] + c[Y] * c[Y] + c[Z] * c[Z]);
}

inline void cgvPoint3D::normalize() {
    float len = length();
    if (len > CGV_EPSILON) {
        c[X] /= len;
        c[Y] /= len;
        c[Z] /= len;
    }
}

inline const float* cgvPoint3D::data() const {
    return c;
}

#endif   // __CGVPOINT3D

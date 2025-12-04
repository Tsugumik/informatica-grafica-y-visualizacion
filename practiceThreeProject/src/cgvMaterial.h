#ifndef __CGVMATERIAL
#define __CGVMATERIAL

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include "cgvColor.h"

class cgvMaterial {
private:
    cgvColor Ka = {0, 0, 0};
    cgvColor Kd = {0, 0, 0};
    cgvColor Ks = {0, 0, 0};
    float Ns = 0; // Shininess (Phong exponent)

public:
    cgvMaterial() = default;
    ~cgvMaterial() = default;

    cgvMaterial(const cgvMaterial& p);
    cgvMaterial(cgvColor _Ka, cgvColor _Kd, cgvColor _Ks, float _Ns);

    void apply();
    void set(cgvColor _Ka, cgvColor _Kd, cgvColor _Ks, float _Ns);
};

#endif // __CGVMATERIAL

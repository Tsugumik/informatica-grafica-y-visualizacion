#if defined(__APPLE__) && defined(__MACH__)

#include <GLUT/glut.h>

#include <OpenGL/gl.h>

#include <OpenGL/glu.h>

#else

#include <GL/glut.h>

#endif

#include <math.h>
#include <stdio.h>

#include "cgvCamera.h"
// Constructor methods
cgvCamera::cgvCamera() {}

cgvCamera::~cgvCamera() {}

cgvCamera::cgvCamera(cameraType _type, cgvPoint3D _P0, cgvPoint3D _r, cgvPoint3D _V) {
    P0 = _P0;
    r = _r;
    V = _V;

    type = _type;
}

void cgvCamera::set(cgvPoint3D _P0, cgvPoint3D _r, cgvPoint3D _V) {
    P0 = _P0;
    r = _r;
    V = _V;
}

void cgvCamera::set(cameraType _type, cgvPoint3D _P0, cgvPoint3D _r, cgvPoint3D _V,
                    double _xwmin, double _xwmax, double _ywmin, double _ywmax, double _znear, double _zfar) {
    type = _type;

    P0 = _P0;
    r = _r;
    V = _V;

    xwmin = _xwmin;
    xwmax = _xwmax;
    ywmin = _ywmin;
    ywmax = _ywmax;
    znear = _znear;
    zfar = _zfar;
}

void cgvCamera::set(cameraType _tipo, cgvPoint3D _P0, cgvPoint3D _r, cgvPoint3D _V,
                    double _angulo, double _raspecto, double _znear, double _zfar) {
    type = _tipo;

    P0 = _P0;
    r = _r;
    V = _V;

    angle = _angulo;
    aspect = _raspecto;
    znear = _znear;
    zfar = _zfar;
}

void cgvCamera::apply(void) {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (type == CGV_PARALLEL) {
        glOrtho(xwmin, xwmax, ywmin, ywmax, znear, zfar);
    }
    if (type == CGV_FRUSTRUM) {
        glFrustum(xwmin, xwmax, ywmin, ywmax, znear, zfar);
    }
    if (type == CGV_PERSPECTIVE) {
        gluPerspective(angle, aspect, znear, zfar);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(P0[X], P0[Y], P0[Z], r[X], r[Y], r[Z], V[X], V[Y], V[Z]);
}

void cgvCamera::zoom(double factor) {
    if (type == CGV_PARALLEL || type == CGV_FRUSTRUM)
    {
        xwmin *= factor;
        xwmax *= factor;
        ywmin *= factor;
        ywmax *= factor;
    }
    else
    {
        if (angle * factor < 180.0)
        {
            angle *= factor;
        }
    }
}
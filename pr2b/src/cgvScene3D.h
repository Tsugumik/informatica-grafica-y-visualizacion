#pragma once

#ifndef __IGVESCENA3D
#define __IGVESCENA3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

class cgvScene3D {
    protected:
    // Attributes
        bool axis;

    public:
    // Default constructors and destructor
    cgvScene3D();
    ~cgvScene3D();

    // Methods
    // Method with OpenGL calls to display the scene
    void display();

    bool get_ejes() { return axis; };
    void set_ejes(bool _axis) { axis = _axis; };
    };

#endif

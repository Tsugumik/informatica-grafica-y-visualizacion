#ifndef MATERIAL_H
#define MATERIAL_H

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Material {
public:
    Material();
    Material(const GLfloat* amb, const GLfloat* diff, const GLfloat* spec, GLfloat shin);

    void apply() const;

private:
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
};

#endif // MATERIAL_H

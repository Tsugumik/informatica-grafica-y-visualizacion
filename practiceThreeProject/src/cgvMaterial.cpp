#include "cgvMaterial.h"

cgvMaterial::cgvMaterial(const cgvMaterial& p) : Ka(p.Ka), Kd(p.Kd), Ks(p.Ks), Ns(p.Ns) {}

cgvMaterial::cgvMaterial(cgvColor _Ka, cgvColor _Kd, cgvColor _Ks, float _Ns) :
    Ka(_Ka), Kd(_Kd), Ks(_Ks), Ns(_Ns) {}

void cgvMaterial::apply() {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ka.data());
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Kd.data());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks.data());
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Ns);

    // Set emission color to (0.0, 0.0, 0.0) (the object is not a light source)
    const GLfloat emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
}

void cgvMaterial::set(cgvColor _Ka, cgvColor _Kd, cgvColor _Ks, float _Ns) {
    Ka = _Ka;
    Kd = _Kd;
    Ks = _Ks;
    Ns = _Ns;
}

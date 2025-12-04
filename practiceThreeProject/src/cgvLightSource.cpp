#include "cgvLightSource.h"

cgvLightSource::cgvLightSource(const unsigned int _idLuz, const cgvPoint3D& _posicion,
                               const cgvColor& cAmb, const cgvColor& cDif, const cgvColor& cEsp,
                               const float a0, const float a1, const float a2) :
    idLuz(_idLuz), position(_posicion), colorAmbient(cAmb), colorDiffuse(cDif), colorSpecular(cEsp),
    aten_a0(a0), aten_a1(a1), aten_a2(a2), on(true) {}

cgvLightSource::cgvLightSource(const unsigned int _idLuz, const cgvPoint3D& _posicion,
                               const cgvColor& cAmb, const cgvColor& cDif, const cgvColor& cEsp,
                               const float a0, const float a1, const float a2,
                               const cgvPoint3D& dir_foco, const float ang_foco, const float exp_foco) :
    idLuz(_idLuz), position(_posicion), colorAmbient(cAmb), colorDiffuse(cDif), colorSpecular(cEsp),
    aten_a0(a0), aten_a1(a1), aten_a2(a2),
    spotlight_direction(dir_foco), spotlight_angle(ang_foco), spotlight_exponent(exp_foco), on(true) {}

cgvPoint3D& cgvLightSource::getPosition() { return position; }
void cgvLightSource::setPosition(cgvPoint3D pos) { position = pos; }

void cgvLightSource::set(const cgvColor& cAmb, const cgvColor& cDif, const cgvColor& cEsp) {
    colorAmbient = cAmb;
    colorDiffuse = cDif;
    colorSpecular = cEsp;
}

void cgvLightSource::setAmbient(const cgvColor& cAmb) { colorAmbient = cAmb; }
void cgvLightSource::setDiffuse(const cgvColor& cDif) { colorDiffuse = cDif; }
void cgvLightSource::setSpecular(const cgvColor& cEsp) { colorSpecular = cEsp; }

cgvColor& cgvLightSource::getAmbient() { return colorAmbient; }
cgvColor& cgvLightSource::getDiffuse() { return colorDiffuse; }
cgvColor& cgvLightSource::getSpecular() { return colorSpecular; }

void cgvLightSource::setAttenuation(float a0, float a1, float a2) {
    aten_a0 = a0;
    aten_a1 = a1;
    aten_a2 = a2;
}

void cgvLightSource::getAttenuation(float& a0, float& a1, float& a2) {
    a0 = aten_a0;
    a1 = aten_a1;
    a2 = aten_a2;
}

void cgvLightSource::turnon() { on = true; }
void cgvLightSource::shutdown() { on = false; }
bool cgvLightSource::is_on() const { return on; }

void cgvLightSource::apply() {
    if (on) {
        glEnable(idLuz);
        glLightfv(idLuz, GL_POSITION, position.data());
        glLightfv(idLuz, GL_AMBIENT, colorAmbient.data());
        glLightfv(idLuz, GL_DIFFUSE, colorDiffuse.data());
        glLightfv(idLuz, GL_SPECULAR, colorSpecular.data());
        glLightf(idLuz, GL_CONSTANT_ATTENUATION, aten_a0);
        glLightf(idLuz, GL_LINEAR_ATTENUATION, aten_a1);
        glLightf(idLuz, GL_QUADRATIC_ATTENUATION, aten_a2);
        glLightfv(idLuz, GL_SPOT_DIRECTION, spotlight_direction.data());
        glLightf(idLuz, GL_SPOT_CUTOFF, spotlight_angle);
        glLightf(idLuz, GL_SPOT_EXPONENT, spotlight_exponent);
    } else {
        glDisable(idLuz);
    }
}

void cgvLightSource::move(float dx, float dy, float dz) {
    position[0] += dx;
    position[1] += dy;
    position[2] += dz;
}

#ifndef __IGVFUENTELUZ
#define __IGVFUENTELUZ

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include "cgvPoint3D.h"
#include "cgvColor.h"

class cgvLightSource {
private:
    unsigned int idLuz = 0;
    cgvPoint3D position = {0, 0, 0};
    cgvColor colorAmbient = {0, 0, 0};
    cgvColor colorDiffuse = {0, 0, 0};
    cgvColor colorSpecular = {0, 0, 0};

    float aten_a0 = 1.0f;
    float aten_a1 = 0.0f;
    float aten_a2 = 0.0f;

    cgvPoint3D spotlight_direction = {0, 0, -1};
    float spotlight_angle = 180.0f;
    float spotlight_exponent = 0.0f;
    bool on = false;

public:
    cgvLightSource() = default;
    ~cgvLightSource() = default;

    cgvLightSource(const unsigned int _LightID, const cgvPoint3D& _position,
                   const cgvColor& cAmb, const cgvColor& cDif, const cgvColor& cEsp,
                   const float a0, const float a1, const float a2);

    cgvLightSource(const unsigned int _LightID, const cgvPoint3D& _position,
                   const cgvColor& cAmb, const cgvColor& cDif, const cgvColor& cEsp,
                   const float a0, const float a1, const float a2,
                   const cgvPoint3D& focus_dir, const float ang_focus, const float exp_focus);

    cgvPoint3D& getPosition();
    void setPosition(cgvPoint3D pos);

    void set(const cgvColor& cAmb, const cgvColor& cDif, const cgvColor& cEsp);
    void setAmbient(const cgvColor& cAmb);
    void setDiffuse(const cgvColor& cDif);
    void setSpecular(const cgvColor& cEsp);
    cgvColor& getAmbient();
    cgvColor& getDiffuse();
    cgvColor& getSpecular();

    void setAttenuation(float a0, float a1, float a2);
    void getAttenuation(float& a0, float& a1, float& a2);

    void turnon();
    void shutdown();
    bool is_on() const;

    void apply();
    void move(float dx, float dy, float dz);
};

#endif // __IGVFUENTELUZ

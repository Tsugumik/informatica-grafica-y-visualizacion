#ifndef __CGVCOLOR
#define __CGVCOLOR

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

enum cColor { R, G, B, A };

class cgvColor {
protected:
    float color[4] = {0.0f, 0.0f, 0.0f, 1.0f};

public:
    cgvColor() = default;
    ~cgvColor() = default;

    cgvColor(const float r, const float g, const float b);
    cgvColor(const float r, const float g, const float b, const float a);
    cgvColor(const cgvColor& p);

    float& operator[](const unsigned char idx);
    const float& operator[](const unsigned char idx) const;

    void apply();
    const float* data() const;
};

inline float& cgvColor::operator[](const unsigned char idx) { return color[idx]; }
inline const float& cgvColor::operator[](const unsigned char idx) const { return color[idx]; }
inline const float* cgvColor::data() const { return color; }

#endif // __CGVCOLOR

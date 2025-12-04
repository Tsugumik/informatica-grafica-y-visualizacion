#ifndef TEXTURE_H
#define TEXTURE_H

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string>
// #include "stb_image.h" // Removed

class Texture {
public:
    Texture();
    ~Texture();

    bool load(const std::string& filename);
    void bind() const;
    void unbind() const;
    void setFilters(GLint minFilter, GLint magFilter);

private:
    GLuint textureID;
};

#endif // TEXTURE_H

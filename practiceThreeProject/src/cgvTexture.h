#ifndef __CGVTEXTURE
#define __CGVTEXTURE

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/glut.h>

#endif   // defined(__APPLE__) && defined(__MACH__)

#include <string>

/**
* Objects of this class represent OpenGL textures
*/
class cgvTexture
{
    private:

    // Attributes
    unsigned int idTexture = 0; // OpenGL texture identifier
    unsigned int height = 0, // Texture height
    width = 0; // Texture width

    // Methods

   public:
    // Default constructors and destructor
    // Default constructor
    cgvTexture() = default;
    cgvTexture(std::string file); // Texture loaded from a file
    ~cgvTexture();

    // Methods
    void apply(); // Sets the texture as the active one
    void setTextureId(unsigned int id);
    unsigned int getTextureId();
};

#endif   // __CGVTEXTURE

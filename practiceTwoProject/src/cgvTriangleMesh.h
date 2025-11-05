#ifndef __IGVMALLATRIANGULOS
#define __IGVMALLATRIANGULOS

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/glut.h>

#endif   // defined(__APPLE__) && defined(__MACH__)

#include <string>

/**
 * Objects of this class represent triangle meshes.
 */
class cgvTriangleMesh
{
protected:
    // Attributes
    long int num_vertices = 0; ///< Number of vertices in the triangle mesh
    float* vertices = nullptr; ///< Array with the (num_vertices * 3) coordinates of the vertices
    float* normals = nullptr;
    ///< Array with the (num_vertices * 3) coordinates of the normal at each vertex (only for sphere generation)

    long int num_triangles = 0; ///< Number of triangles in the triangle mesh
    unsigned int* triangles = nullptr; ///< Array with the (num_triangles * 3) indices to the vertices of each triangle
    bool usenormals;
    bool gouraud;

public:
    // Constructor and destructor
    /// Default constructor
    cgvTriangleMesh() = default;

    cgvTriangleMesh(long int _num_vertices, float* _vertices
                       , long int _num_triangles, unsigned int* _triangles);

    ~cgvTriangleMesh();

    // Method with OpenGL calls to display the triangle mesh
    void display();

    void changeNormals() { usenormals = !usenormals; }
    void changeVis() { gouraud = !gouraud; }
};

#endif   // __IGVMALLATRIANGULOS

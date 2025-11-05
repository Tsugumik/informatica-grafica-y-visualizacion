#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include "cgvTriangleMesh.h"

/**
 * Parameterised constructor of a triangle mesh without normals at the
 * vertices
 * @param _num_vertices Number of vertices in the new mesh
 * @param _vertices Memory address where the
 *        coordinates (X,Y,Z format) of the vertices are stored. This information is
 *        copied to the new mesh
 * @param _num_triangles Number of triangles that form the mesh
 * @param _triangles Indices (v1, v2, v3 format) to the vertices that form
 *        each triangle. This information is copied to the new object
 * @pre It is assumed that all parameters have valid values
 * @post The new mesh will store copies of the information passed to it as a
 *       parameter
 */
cgvTriangleMesh::cgvTriangleMesh(long int _num_vertex, float* _vertex
                                       , long int _num_triangles
                                       , unsigned int* _triangles) :
    num_vertices(_num_vertex)
    , num_triangles(_num_triangles)
{
    num_vertices = _num_vertex;
    vertices = new float[num_vertices * 3];
    for (long int i = 0; i < (num_vertices * 3); ++i)
    {
        vertices[i] = _vertex[i];
    }

    normals = nullptr;

    num_triangles = _num_triangles;
    _triangles = new unsigned int[num_triangles * 3];
    for (long int i = 0; i < (num_triangles * 3); ++i)
    {
        triangles[i] = _triangles[i];
    }
}

/**
 * Destructor
 */
cgvTriangleMesh::~cgvTriangleMesh()
{
    if (vertices)
    {
        delete []vertices;
        vertices = nullptr;
    }

    if (normals)
    {
        delete []normals;
        normals = nullptr;
    }

    if (triangles)
    {
        delete []triangles;
        triangles = nullptr;
    }
}

/**
 * Method with OpenGL calls to display the triangle mesh
 */
void cgvTriangleMesh::display()
{
    if (!gouraud)
    {
        glShadeModel(GL_FLAT);
    }
    else
    {
        glShadeModel(GL_SMOOTH);
    }

    glEnable(GL_NORMALIZE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    if (usenormals)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, normals);
    }

    glDrawElements(GL_TRIANGLES, num_triangles * 3, GL_UNSIGNED_INT, triangles);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

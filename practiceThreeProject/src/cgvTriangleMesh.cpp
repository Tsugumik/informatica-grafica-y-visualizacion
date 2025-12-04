#include "cgvTriangleMesh.h"

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void cgvTriangleMesh::draw() {
    glPushMatrix();
    applyTransformations();

    if (material) {
        material->apply();
    }

    bool hasTexture = texture && !tex_coords.empty();

    if (hasTexture) {
        glEnable(GL_TEXTURE_2D);
        texture->apply();
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        // The stride is the size of the full cgvPoint3D struct, as our data is stored as an array of these structs.
        glTexCoordPointer(3, GL_FLOAT, sizeof(cgvPoint3D), (const GLvoid*)tex_coords.data());
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // The stride is the size of the full cgvPoint3D struct.
    glVertexPointer(3, GL_FLOAT, sizeof(cgvPoint3D), (const GLvoid*)vertices.data());
    glNormalPointer(GL_FLOAT, sizeof(cgvPoint3D), (const GLvoid*)normals.data());

    glDrawElements(GL_TRIANGLES, triangles.size() * 3, GL_UNSIGNED_INT, (const GLvoid*)triangles.data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    if (hasTexture) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();
}

void cgvTriangleMesh::compute_normals() {
    if (vertices.empty() || triangles.empty()) return;

    normals.assign(vertices.size(), cgvPoint3D(0, 0, 0, 0));

    for (const auto& tri : triangles) {
        cgvPoint3D v0 = vertices[tri.v[0]];
        cgvPoint3D v1 = vertices[tri.v[1]];
        cgvPoint3D v2 = vertices[tri.v[2]];

        cgvPoint3D edge1 = v1 - v0;
        cgvPoint3D edge2 = v2 - v0;
        cgvPoint3D normal = edge1.cross(edge2);
        
        normals[tri.v[0]] += normal;
        normals[tri.v[1]] += normal;
        normals[tri.v[2]] += normal;
    }

    for (auto& normal : normals) {
        normal.normalize();
    }
}

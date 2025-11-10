#include "cgvTriangleMesh.h"

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void cgvTriangleMesh::draw() {
    glPushMatrix();
    applyTransformations();

    GLfloat specular[] = { specular_reflectivity, specular_reflectivity, specular_reflectivity, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glColor3f(0.6f, 0.6f, 0.8f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices.data());
    glNormalPointer(GL_FLOAT, 0, normals.data());

    glDrawElements(GL_TRIANGLES, triangles.size() * 3, GL_UNSIGNED_INT, triangles.data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    GLfloat default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);

    glPopMatrix();
}

void cgvTriangleMesh::compute_normals() {
    if (vertices.empty() || triangles.empty()) return;

    normals.assign(vertices.size(), cgvPoint3D(0, 0, 0));

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

#ifndef __CGV_TRIANGLE_MESH_H
#define __CGV_TRIANGLE_MESH_H

#include <vector>
#include "cgvPoint3D.h"
#include "Object3D.h"

class cgvTriangle {
public:
    unsigned int v[3];
    cgvTriangle(unsigned int v0, unsigned int v1, unsigned int v2) {
        v[0] = v0; v[1] = v1; v[2] = v2;
    }
};

class cgvTriangleMesh : public Object3D {
protected:
    std::vector<cgvPoint3D> vertices;
    std::vector<cgvPoint3D> normals;
    std::vector<cgvTriangle> triangles;

    float specular_reflectivity = 0.5f;
    float shininess = 10.0f;

public:
    cgvTriangleMesh() = default;
    ~cgvTriangleMesh() = default;

    void draw() override;
    void compute_normals();

    std::vector<cgvPoint3D>& get_vertices() { return vertices; }
    std::vector<cgvPoint3D>& get_normals() { return normals; }
    std::vector<cgvTriangle>& get_triangles() { return triangles; }

    void set_specular_reflectivity(float reflectivity) { specular_reflectivity = reflectivity; }
    void set_shininess(float s) { shininess = s; }
};

#endif

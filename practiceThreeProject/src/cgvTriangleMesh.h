#ifndef __CGV_TRIANGLE_MESH_H
#define __CGV_TRIANGLE_MESH_H

#include <vector>
#include "cgvPoint3D.h"
#include "Object3D.h"
#include "cgvMaterial.h"
#include "cgvTexture.h"

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
    std::vector<cgvPoint3D> tex_coords;
    std::vector<cgvTriangle> triangles;

    cgvMaterial* material = nullptr;
    cgvTexture* texture = nullptr;

public:
    cgvTriangleMesh() = default;
    ~cgvTriangleMesh() = default;

    void draw() override;
    void compute_normals();

    std::vector<cgvPoint3D>& get_vertices() { return vertices; }
    std::vector<cgvPoint3D>& get_normals() { return normals; }
    std::vector<cgvPoint3D>& get_tex_coords() { return tex_coords; }
    std::vector<cgvTriangle>& get_triangles() { return triangles; }

    void set_material(cgvMaterial* mat) { material = mat; }
    void set_texture(cgvTexture* tex) { texture = tex; }
};

#endif

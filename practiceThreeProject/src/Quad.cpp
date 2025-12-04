#include "Quad.h"

Quad::Quad(float width, float depth) {
    float half_width = width / 2.0f;
    float half_depth = depth / 2.0f;

    // Vertices
    vertices.emplace_back(-half_width, 0.0f, half_depth);  // 0
    vertices.emplace_back(half_width, 0.0f, half_depth);   // 1
    vertices.emplace_back(half_width, 0.0f, -half_depth);  // 2
    vertices.emplace_back(-half_width, 0.0f, -half_depth); // 3

    // Normals (all pointing up)
    normals.emplace_back(0.0f, 1.0f, 0.0f);
    normals.emplace_back(0.0f, 1.0f, 0.0f);
    normals.emplace_back(0.0f, 1.0f, 0.0f);
    normals.emplace_back(0.0f, 1.0f, 0.0f);

    // Triangles
    triangles.emplace_back(0, 1, 2);
    triangles.emplace_back(0, 2, 3);

    // Texture coordinates
    tex_coords.emplace_back(0.0f, 1.0f, 0.0f);
    tex_coords.emplace_back(1.0f, 1.0f, 0.0f);
    tex_coords.emplace_back(1.0f, 0.0f, 0.0f);
    tex_coords.emplace_back(0.0f, 0.0f, 0.0f);
}

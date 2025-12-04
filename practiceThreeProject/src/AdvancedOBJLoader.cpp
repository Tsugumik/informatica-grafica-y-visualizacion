#include "AdvancedOBJLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

struct VertexTuple {
    unsigned int v_idx, n_idx;
    bool operator<(const VertexTuple& other) const {
        if (v_idx != other.v_idx) return v_idx < other.v_idx;
        return n_idx < other.n_idx;
    }
};

static void process_face_data(
    std::stringstream& line_ss,
    const std::vector<cgvPoint3D>& temp_v, const std::vector<cgvPoint3D>& temp_n,
    cgvTriangleMesh& mesh, std::map<VertexTuple, unsigned int>& vertex_map) {

    std::vector<unsigned int> face_indices;
    for (int i = 0; i < 3; ++i) {
        std::string face_part;
        line_ss >> face_part;
        std::stringstream part_ss(face_part);
        
        VertexTuple tuple = {0, 0};
        part_ss >> tuple.v_idx;
        if (part_ss.str().find("//") != std::string::npos) {
            part_ss.ignore(2);
            part_ss >> tuple.n_idx;
        }

        auto it = vertex_map.find(tuple);
        if (it != vertex_map.end()) {
            face_indices.push_back(it->second);
        } else {
            unsigned int new_idx = mesh.get_vertices().size();
            mesh.get_vertices().push_back(temp_v[tuple.v_idx - 1]);
            if (tuple.n_idx > 0) {
                mesh.get_normals().push_back(temp_n[tuple.n_idx - 1]);
            }
            vertex_map[tuple] = new_idx;
            face_indices.push_back(new_idx);
        }
    }
    mesh.get_triangles().emplace_back(face_indices[0], face_indices[1], face_indices[2]);
}

bool AdvancedOBJLoader::load(const std::string& path, cgvTriangleMesh& mesh) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::vector<cgvPoint3D> temp_v, temp_n;
    std::map<VertexTuple, unsigned int> vertex_map;
    mesh.get_vertices().clear();
    mesh.get_normals().clear();
    mesh.get_triangles().clear();

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;
        if (prefix == "v") {
            cgvPoint3D v; ss >> v[X] >> v[Y] >> v[Z];
            temp_v.push_back(v);
        } else if (prefix == "vn") {
            cgvPoint3D n; ss >> n[X] >> n[Y] >> n[Z];
            temp_n.push_back(n);
        } else if (prefix == "f") {
            process_face_data(ss, temp_v, temp_n, mesh, vertex_map);
        }
    }

    if (mesh.get_normals().empty()) {
        mesh.compute_normals();
    }
    return true;
}

bool AdvancedOBJLoader::load_articulated(const std::string& path, std::map<std::string, cgvTriangleMesh*>& meshes) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::vector<cgvPoint3D> temp_v, temp_n;
    cgvTriangleMesh* current_mesh = nullptr;
    std::string current_name = "default";
    std::map<VertexTuple, unsigned int> vertex_map;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "o" || prefix == "g") {
            ss >> current_name;
            if (meshes.find(current_name) == meshes.end()) {
                meshes[current_name] = new cgvTriangleMesh();
            }
            current_mesh = meshes[current_name];
            vertex_map.clear();
        } else if (prefix == "v") {
            cgvPoint3D v; ss >> v[X] >> v[Y] >> v[Z];
            temp_v.push_back(v);
        } else if (prefix == "vn") {
            cgvPoint3D n; ss >> n[X] >> n[Y] >> n[Z];
            temp_n.push_back(n);
        } else if (prefix == "f") {
            if (!current_mesh) {
                if (meshes.find(current_name) == meshes.end()) {
                    meshes[current_name] = new cgvTriangleMesh();
                }
                current_mesh = meshes[current_name];
            }
            process_face_data(ss, temp_v, temp_n, *current_mesh, vertex_map);
        }
    }

    for (auto const& [name, mesh] : meshes) {
        if (mesh->get_normals().empty()) {
            mesh->compute_normals();
        }
    }
    return true;
}

#ifndef ADVANCED_OBJ_LOADER_H
#define ADVANCED_OBJ_LOADER_H

#include <string>
#include <vector>
#include <map>
#include "cgvTriangleMesh.h"

class AdvancedOBJLoader {
public:
    // Loads a single mesh from an OBJ file.
    static bool load(const std::string& path, cgvTriangleMesh& mesh);

    // Loads multiple meshes from a single OBJ file, separating them by object/group tags.
    static bool load_articulated(const std::string& path, std::map<std::string, cgvTriangleMesh*>& meshes);
};

#endif // ADVANCED_OBJ_LOADER_H

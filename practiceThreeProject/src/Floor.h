#ifndef FLOOR_H
#define FLOOR_H

#include "Object3D.h"
#include "Material.h"
#include "Texture.h"
#include <vector>
#include <memory>

class Floor : public Object3D {
public:
    Floor(float size = 20.0f);
    void init(); // New method
    void draw() override;
    void setMaterial(int materialIndex);
    void toggleTexture(bool enable);
    void setTexture(int textureIndex);
    void setTextureFilters(int filterType);

private:
    void createMaterials();
    void loadTextures();

    float _size;
    std::vector<Material> materials;
    int currentMaterialIndex;

    bool textureEnabled;
    std::vector<std::unique_ptr<Texture>> textures;
    int currentTextureIndex;
};

#endif // FLOOR_H

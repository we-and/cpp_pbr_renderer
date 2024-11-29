#ifndef MAT_H
#define MAT_H
struct Material {
    glm::vec3 albedo{0.5f};    // Base color of the material
    float metallic{0.0f};       // How metallic the material is (0-1)
    float roughness{0.5f};      // Surface roughness (0-1)
    float ao{1.0f};            // Ambient occlusion factor (0-1)
};

#endif
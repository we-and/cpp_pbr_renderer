#ifndef AV_H
#define AV_H
#include <glm/glm.hpp>

#include <vector>
struct AxisVertex {
    glm::vec3 position;
    glm::vec3 color;
};

std::vector<AxisVertex> generateScaledAxes(float scale) {
    return {
        // X-axis (Red)
        {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
        {glm::vec3(scale, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},

        // Y-axis (Green)
        {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
        {glm::vec3(0.0f, scale, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},

        // Z-axis (Blue)
        {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3(0.0f, 0.0f, scale), glm::vec3(0.0f, 0.0f, 1.0f)}
    };
}

#endif
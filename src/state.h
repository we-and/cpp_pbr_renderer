#ifndef STATE_H
#define STATE_H
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class State
{
public:
    float yaw = -90.0f;             // Yaw angle, initially pointing along -Z
    float pitch = 0.0f;             // Pitch angle
    float lastX = 400, lastY = 300; // Last mouse position
    float sensitivity = 0.1f;       // Mouse sensitivity
    bool firstMouse = true;
    float rotationX = 0.0f; // Rotation angle around the X-axis (pitch)
    float rotationY = 0.0f; // Rotation angle around the Y-axis (yaw)

    std::vector<glm::vec3> transformedLightPositions;

    float distance = 50.f;                                // First mouse movement flag
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // Initial camera direction
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);     // World up vector
    glm::vec3 cameraPos = glm::vec3(50.0f, 20.0f, 50.0f);   // Initial camera position
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    float cameraDistance=50;
    //
    GLuint axisVAO, lightbboxVAO;
    GLuint gridVAO;
    GLuint scenebboxVAO, scenebboxVBO, scenebboxEBO;
};
#endif
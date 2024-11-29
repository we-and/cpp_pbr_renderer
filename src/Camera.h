#ifndef CAM
#define CAM

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <tiny_gltf.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

#include <glm/glm.hpp>
#include "state.h"
#include <vector>
class Camera{
    public:
static void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    static float lastX = xpos, lastY = ypos;

    State *state = static_cast<State *>(glfwGetWindowUserPointer(window));
    if (!state)
        return;

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos; // Reversed for pitch
    lastX = xpos;
    lastY = ypos;

    xOffset *= state->sensitivity;
    yOffset *= state->sensitivity;

    state->rotationX += yOffset; // Update pitch (X-axis rotation)
    state->rotationY += xOffset; // Update yaw (Y-axis rotation)
}

 static    void mouse_callback_cam(GLFWwindow *window, double xpos, double ypos)
{
    // Retrieve the State pointer from the user pointer
    State *state = static_cast<State *>(glfwGetWindowUserPointer(window));
    if (!state)
        return;

    if (state->firstMouse)
    {
        state->lastX = xpos;
        state->lastY = ypos;
        state->firstMouse = false;
    }

    float xOffset = xpos - state->lastX;
    float yOffset = state->lastY - ypos; // Reversed since y-coordinates go from bottom to top
    state->lastX = xpos;
    state->lastY = ypos;

    xOffset *= state->sensitivity;
    yOffset *= state->sensitivity;

    state->yaw += xOffset;
    state->pitch += yOffset;

    // Constrain the pitch to prevent flipping
    if (state->pitch > 89.0f)
        state->pitch = 89.0f;
    if (state->pitch < -89.0f)
        state->pitch = -89.0f;

    // Update camera direction
    glm::vec3 front;
    front.x = cos(glm::radians(state->yaw)) * cos(glm::radians(state->pitch));
    front.y = sin(glm::radians(state->pitch));
    front.z = sin(glm::radians(state->yaw)) * cos(glm::radians(state->pitch));
    state->cameraFront = glm::normalize(front);
//std::cout <<"camera"<<state->pitch<<" "<<state->yaw<<" "<<state->rotationX<<" "<<state->rotationY<<std::endl;

}
static glm::vec3 computeHorizontalCameraPosition(float distance, float rotationY, float altitude, const glm::vec3& sceneCenter) {
    float x = sceneCenter.x + distance * cos(glm::radians(rotationY));
    float z = sceneCenter.z + distance * sin(glm::radians(rotationY));
    return glm::vec3(x, altitude, z);
}
static void mouse_callback_orbit(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = xpos, lastY = ypos;

    State* state = static_cast<State*>(glfwGetWindowUserPointer(window));
    if (!state) return;

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos; // Reversed for pitch
    lastX = xpos;
    lastY = ypos;

    xOffset *= state->sensitivity;
    yOffset *= state->sensitivity;

    state->rotationY += xOffset; // Yaw (horizontal rotation)
    state->rotationX += yOffset; // Pitch (vertical rotation)

    // Clamp pitch to avoid flipping
    state->rotationX = glm::clamp(state->rotationX, -89.0f, 89.0f);
}

    static glm::vec3 computeCameraPosition(float distance, float rotationX, float rotationY, const glm::vec3& sceneCenter) {
    float x = sceneCenter.x + distance * cos(glm::radians(rotationY)) * cos(glm::radians(rotationX));
    float y = sceneCenter.y + distance * sin(glm::radians(rotationX));
    float z = sceneCenter.z + distance * sin(glm::radians(rotationY)) * cos(glm::radians(rotationX));
    return glm::vec3(x, y, z);
}


static void mouse_callback_camera_left_right(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = xpos;

    State* state = static_cast<State*>(glfwGetWindowUserPointer(window));
    if (!state) return;

    float xOffset = xpos - lastX;
    lastX = xpos;

    xOffset *= state->sensitivity;

    state->rotationY += xOffset; // Yaw (horizontal rotation)

   // std::cout << "RotationY: " << state->rotationY << "\n";
}
};
#endif
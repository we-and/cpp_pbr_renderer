#ifndef AXIS_H
#define AXIS_H
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

#include <vector>
#include "AxisVertex.h"

class Axis{
public:
static 
GLuint setupAxisVAO(const std::vector<AxisVertex>& vertices) {
    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(AxisVertex), vertices.data(), GL_STATIC_DRAW);

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AxisVertex), (void*)offsetof(AxisVertex, position));
    glEnableVertexAttribArray(0);

    // Color attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AxisVertex), (void*)offsetof(AxisVertex, color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    return VAO;
}
static std::vector<AxisVertex> generateScaledAxes(float scale) {
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
static 
void renderAxes(GLuint axisVAO, const glm::mat4& view, const glm::mat4& projection,const glm::mat4& transform, GLuint shaderProgram) {
    glUseProgram(shaderProgram);

    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &transform[0][0]);

    glBindVertexArray(axisVAO);
    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);
}

};
#endif
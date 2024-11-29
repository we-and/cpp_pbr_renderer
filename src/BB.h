#ifndef BB_H
#define BB_H
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

class BB {
public:
static 
std::vector<glm::vec3> generateBoundingBoxVertices(const glm::vec3 &min, const glm::vec3 &max)
{
    return {
        glm::vec3(min.x, min.y, min.z), // Bottom-left-front
        glm::vec3(max.x, min.y, min.z), // Bottom-right-front
        glm::vec3(max.x, max.y, min.z), // Top-right-front
        glm::vec3(min.x, max.y, min.z), // Top-left-front
        glm::vec3(min.x, min.y, max.z), // Bottom-left-back
        glm::vec3(max.x, min.y, max.z), // Bottom-right-back
        glm::vec3(max.x, max.y, max.z), // Top-right-back
        glm::vec3(min.x, max.y, max.z)  // Top-left-back
    };
}
static std::vector<unsigned int> getBoundingBoxLineIndices()
{
    return {
        0, 1, 1, 2, 2, 3, 3, 0, // Front face
        4, 5, 5, 6, 6, 7, 7, 4, // Back face
        0, 4, 1, 5, 2, 6, 3, 7  // Connecting edges
    };
}
inline static std::vector<glm::vec3> cubeVertices = {
    {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, // Back face
    {-0.5f, -0.5f, 0.5f},  {0.5f, -0.5f, 0.5f},  {0.5f, 0.5f, 0.5f},  {-0.5f, 0.5f, 0.5f}   // Front face
};


inline static std::vector<unsigned int> cubeIndices = {
    0, 1, 2, 2, 3, 0, // Back face
    4, 5, 6, 6, 7, 4, // Front face
    0, 4, 7, 7, 3, 0, // Left face
    1, 5, 6, 6, 2, 1, // Right face
    3, 7, 6, 6, 2, 3, // Top face
    0, 4, 5, 5, 1, 0  // Bottom face
};

static GLuint setupBoundingBoxVAO(const std::vector<glm::vec3> &vertices, const std::vector<unsigned int> &indices)
{
    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return VAO;
}
static void renderSceneBB(GLuint VAO, const glm::mat4 &view, const glm::mat4 &projection, const glm::mat4 &transform, GLuint programID)
{
    glUseProgram(programID);

    GLint viewLoc = glGetUniformLocation(programID, "view");
    GLint projectionLoc = glGetUniformLocation(programID, "projection");
    GLint modelLoc = glGetUniformLocation(programID, "model");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);             // Upload the view matrix
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]); // Upload the projection matrix
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &transform[0][0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

static 
void renderLightCubes(GLuint cubeVAO, const std::vector<glm::vec3>& lightPositions, const glm::mat4& view,
                      const glm::mat4& projection,     const glm::mat4& transform, GLuint shaderProgram, float cubeScale = 0.2f) {
    glUseProgram(shaderProgram);

    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(cubeVAO);

    for (const auto& lightPos : lightPositions) {
        // Scale and translate the cube to the light position
        glm::mat4 model = glm::translate(glm::mat4(1.0f), lightPos);
        model = glm::scale(model, glm::vec3(cubeScale));
        model=transform*model;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

        glDrawElements(GL_TRIANGLES, BB:: cubeIndices.size(), GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}
static GLuint setupCubeVAO(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices) {
    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return VAO;
}
};
#endif
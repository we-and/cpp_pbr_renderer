#ifndef GRID_H
#define GRID_H
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

class Grid
{
public:
    static void renderGrid(GLuint gridVAO, const glm::mat4 &view, const glm::mat4 &projection,const glm::mat4 &transform, GLuint shaderProgram, int lines)
    {
        glUseProgram(shaderProgram);

        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");

        glm::mat4 model = transform; // Identity matrix for the grid
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

        glBindVertexArray(gridVAO);

        // Total vertices = 2 * (lines + 1) for X + 2 * (lines + 1) for Z
        glDrawArrays(GL_LINES, 0, 4 * (lines + 1));

        glBindVertexArray(0);
    }

    static std::vector<glm::vec3> generateGridVertices(int lines, float spacing)
    {
        std::vector<glm::vec3> vertices;

        // Adjust lines to include both positive and negative directions around the center
        float halfSize = (lines / 2.0f) * spacing; // Half the size of the grid

        // Generate lines along the X-axis
        for (int i = -lines / 2; i <= lines / 2; ++i)
        {
            float position = i * spacing;
            vertices.emplace_back(glm::vec3(position, 0.0f, -halfSize)); // Line start
            vertices.emplace_back(glm::vec3(position, 0.0f, halfSize));  // Line end
        }

        // Generate lines along the Z-axis
        for (int i = -lines / 2; i <= lines / 2; ++i)
        {
            float position = i * spacing;
            vertices.emplace_back(glm::vec3(-halfSize, 0.0f, position)); // Line start
            vertices.emplace_back(glm::vec3(halfSize, 0.0f, position));  // Line end
        }

        return vertices;
    }

    static GLuint setupGridVAO(const std::vector<glm::vec3> &vertices)
    {
        GLuint VAO, VBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        return VAO;
    }
};

#endif
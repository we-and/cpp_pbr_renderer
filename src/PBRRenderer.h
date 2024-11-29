#ifndef PBR_H
#define PBR_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Material.h"
#include "Model.h"
#include "state.h"
class ShaderLoader
{
public:
    static std::string loadShaderFromFile(const std::string &path)
    {
        std::ifstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str() + "\n"; // Ensure proper termination
    }
};

class PBRRenderer
{
private:
    // Shader program handles
    GLuint pbrShader;

public:

 // Set light positions and colors
        std::vector<glm::vec3> lightPositions = {
        //    glm::vec3(-10.0f, 10.0f, 10.0f),
//            glm::vec3(10.0f, 10.0f, 10.0f),
  //          glm::vec3(-10.0f, -10.0f, 10.0f),
            glm::vec3(-10.0f, 10.0f, 10.0f)};
        std::vector<glm::vec3> lightColors = {
          //  glm::vec3(300.0f),
    //        glm::vec3(300.0f),
      //      glm::vec3(300.0f),
            glm::vec3(300.0f)};

    GLuint bboxShader;
    GLuint axisShader;
    
void initBBoxShader(){
        std::string vertexSource = ShaderLoader::loadShaderFromFile("../shaders/bbox.vert");
        std::string fragmentSource = ShaderLoader::loadShaderFromFile("../shaders/bbox.frag");

        const char *vertexShaderSource = vertexSource.c_str();
        const char *fragmentShaderSource = fragmentSource.c_str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);

        // Check errors
        checkShaderErrors(vertexShader, "vert");
        checkShaderErrors(fragmentShader, "frag");

        // Create shader program
        bboxShader = glCreateProgram();
        glAttachShader(bboxShader, vertexShader);
        glAttachShader(bboxShader, fragmentShader);
        glLinkProgram(bboxShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
        }

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
        }
    
}
void initAxisShader(){
        std::string vertexSource = ShaderLoader::loadShaderFromFile("../shaders/axis.vert");
        std::string fragmentSource = ShaderLoader::loadShaderFromFile("../shaders/axis.frag");

        const char *vertexShaderSource = vertexSource.c_str();
        const char *fragmentShaderSource = fragmentSource.c_str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);

        // Check errors
        checkShaderErrors(vertexShader, "vert");
        checkShaderErrors(fragmentShader, "frag");

        // Create shader program
        axisShader = glCreateProgram();
        glAttachShader(axisShader, vertexShader);
        glAttachShader(axisShader, fragmentShader);
        glLinkProgram(axisShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
        }

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
        }
    
}
    void checkShaderErrors(GLuint shader, const std::string &type)
    {
        GLint success;
        GLchar infoLog[1024];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << type << " shader error:\n"
                      << infoLog << std::endl;
            //   std::cout << "Shader source:\n" << ShaderLoader::loadShaderFromFile("../shaders/pbr." + type ) << std::endl;

            // Get shader source
            GLint length;
            glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &length);
            std::vector<GLchar> source(length);
            glGetShaderSource(shader, length, NULL, source.data());
            // std::cout << "Actual shader source:\n" << source.data() << std::endl;
        }
    }
    void init()
    {
        std::string vertexSource = ShaderLoader::loadShaderFromFile("../shaders/pbr.vert");
        std::string fragmentSource = ShaderLoader::loadShaderFromFile("../shaders/pbr.frag");

        const char *vertexShaderSource = vertexSource.c_str();
        const char *fragmentShaderSource = fragmentSource.c_str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);

        // Check errors
        checkShaderErrors(vertexShader, "vert");
        checkShaderErrors(fragmentShader, "frag");

        // Create shader program
        pbrShader = glCreateProgram();
        glAttachShader(pbrShader, vertexShader);
        glAttachShader(pbrShader, fragmentShader);
        glLinkProgram(pbrShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
        }

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
        }
    }
    void render(const std::shared_ptr<State> &g, const ModelData &m, const glm::mat4 &projection, const glm::mat4 &view, const glm::mat4 &model,
                const glm::vec3 &cameraPos, const glm::vec4 &baseColorFactor,
                float metallicFactor, float roughnessFactor)
    {

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glUseProgram(pbrShader);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glUniformMatrix4fv(glGetUniformLocation(pbrShader, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(pbrShader, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(pbrShader, "model"), 1, GL_FALSE, &model[0][0]);
        glUniform3fv(glGetUniformLocation(pbrShader, "camPos"), 1, &cameraPos[0]);
        glUniform4fv(glGetUniformLocation(pbrShader, "baseColorFactor"), 1, &baseColorFactor[0]);
        glUniform1f(glGetUniformLocation(pbrShader, "metallicFactor"), metallicFactor);
        glUniform1f(glGetUniformLocation(pbrShader, "roughnessFactor"), roughnessFactor);
        if (m.baseColorTexture == 0)
        {
            std::cerr << "Error: Texture not loaded for model." << std::endl;
        }

        if (m.baseColorTexture != 0)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m.baseColorTexture);
            glUniform1i(glGetUniformLocation(pbrShader, "baseColorTexture"), 0);
        }

        // Light positions and colors setup remains the same

       glUniform3fv(glGetUniformLocation(pbrShader, "lightPositions"), 
             g->transformedLightPositions.size(), 
             &g->transformedLightPositions[0][0]);
     //   glUniform3fv(glGetUniformLocation(pbrShader, "lightPositions"), 4, &lightPositions[0][0]);
        glUniform3fv(glGetUniformLocation(pbrShader, "lightColors"), 4, &lightColors[0][0]);

        // In render loop
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cout << "OpenGL error: " << error << std::endl;
        }
    }
};

#endif
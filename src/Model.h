#ifndef MOD_H
#define MOD_H

#include <tiny_gltf.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
struct ModelData
{

    //            tinygltf::Model gltfModel;
    GLuint VAO, VBO, EBO;
    GLuint bboxVAO, bboxVBO, bboxEBO;
    GLuint axisVAO, axisVBO, axisEBO;
    size_t indexCount;
    // int meshIdInGLTF;
    int primitiveIdInGLTFNodeMesh;
    glm::vec3 position;
    glm::vec4 baseColorFactor;
    float metallicFactor;
    float roughnessFactor;
    GLuint baseColorTexture = 0;

    std::shared_ptr<tinygltf::Model> gltfModel = nullptr;
    const tinygltf::Node *gltfNode;
};
#endif
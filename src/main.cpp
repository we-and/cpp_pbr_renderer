#include "PBRRenderer.h"
#include <glad/glad.h>
#include <tiny_gltf.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include "Material.h"
#include "ModelLoad.h"
#include "state.h"
#include "AxisVertex.h"
#include "Camera.h"
#include "BB.h"
#include "Axis.h"
#include "Grid.h"

void cleanupOnExit(std::shared_ptr<State> &g, std::vector<ModelData> modelDatas)
{
    // Cleanup
    for (const auto &model : modelDatas)
    {
        glDeleteVertexArrays(1, &model.VAO);
        glDeleteBuffers(1, &model.VBO);
        glDeleteBuffers(1, &model.EBO);
    }

    glfwTerminate();
}
void processNode(const std::shared_ptr<State> &g ,const ModelData &m, const tinygltf::Node &node, const tinygltf::Model &model,
                 const glm::mat4 &parentTransform, const glm::mat4 &view,
                 const glm::mat4 &projection, const glm::vec3 &cameraPos, PBRRenderer &renderer)
{
    // Compute local transform
    glm::mat4 localTransform = glm::mat4(1.0f);
    if (!node.translation.empty())
    {
        glm::vec3 translation(node.translation[0], node.translation[1], node.translation[2]);
        localTransform = glm::translate(localTransform, translation);
    }
    if (!node.rotation.empty())
    {
        glm::quat rotation(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]);
        localTransform *= glm::mat4_cast(rotation);
    }
    if (!node.scale.empty())
    {
        glm::vec3 scale(node.scale[0], node.scale[1], node.scale[2]);
        localTransform = glm::scale(localTransform, scale);
    }

    // Combine with parent transform
    glm::mat4 modelMatrix = parentTransform * localTransform;
    //                std::cout <<"Process node ["<<node.name<<"] matrix"<<std::endl;

    // Render the mesh if the node contains one
    if (node.mesh >= 0)
    {
        // std::cout <<"Process node  ["<<node.name<<"] has mesh"<<std::endl;
        const auto &mesh = model.meshes[node.mesh];
        //  std::cout <<"Process node  ["<<node.name<<"] has prim "<<mesh.primitives.size()<<std::endl;
        int idx = 0;
        for (int idx = 0; idx < mesh.primitives.size(); ++idx)
        {
            const auto &primitive = mesh.primitives[idx];
            //            std::cout <<"Process node  ["<<node.name<<"] render primitive"<<idx<<"/"<<mesh.primitives.size()<<std::endl;
            // Upload uniforms and render
            renderer.render(g,m, projection, view, modelMatrix, cameraPos,
                            m.baseColorFactor, m.metallicFactor, m.roughnessFactor);
            glBindVertexArray(m.VAO);
            glDrawElements(GL_TRIANGLES, m.indexCount, GL_UNSIGNED_SHORT, 0);
        }
    }

    // Recursively process child nodes
    // std::cout <<"Process node ["<<node.name<<"]  children"<<node.children.size()<<std::endl;
    for (const auto &childIndex : node.children)
    {
        processNode(g,m, model.nodes[childIndex], model, modelMatrix, view, projection, cameraPos, renderer);
    }
}
int main()
{

    // STATE
    std::shared_ptr<State> g = std::make_shared<State>();

    // GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // WINDOW
    GLFWwindow *window = glfwCreateWindow(1400, 900, "PBR Renderer", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    // INPUT
    glfwSetWindowUserPointer(window, g.get());
    glfwSetCursorPosCallback(window, Camera::mouse_callback_camera_left_right);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Disable cursor

    // RENDERER
    PBRRenderer renderer;
    renderer.init();
    renderer.initBBoxShader();
    renderer.initAxisShader();

    // Define models to load with their positions
    std::vector<std::pair<std::string, glm::vec3>> modelPaths = {
        {"../models/Lantern.glb", glm::vec3(0.0f, 0.0f, 0.0f)},
    };

    // MODEL LOADER
    ModelLoader loader;
    std::vector<ModelData> modelDatas = loader.loadModels(modelPaths);
    loader.printModelsInfo();

    // scene dimensions
    glm::vec3 sceneCenter = loader.computeSceneCenter(loader.gltfModels); // Compute scene center
    auto [sceneMin, sceneMax] = loader.computeSceneBounds(loader.gltfModels);
    float sceneRadius = glm::length(sceneMax - sceneMin) * 0.5f;
    int gridSize=(int(sceneRadius)%2==0?int(sceneRadius):int(sceneRadius)+1);
    int gridSpacing=2;
    std::cout << "Scene center" << sceneCenter.x << sceneCenter.y << " " << sceneCenter.z << " " << std::endl;

    // scene bounding box
    std::vector<glm::vec3> boundingBoxVertices = BB::generateBoundingBoxVertices(sceneMin, sceneMax);
    std::vector<unsigned int> boundingBoxIndices = BB::getBoundingBoxLineIndices();
    g->scenebboxVAO = BB::setupBoundingBoxVAO(boundingBoxVertices, boundingBoxIndices);

    // scene grid on the floor at 0,0
    std::vector<glm::vec3> gridVertices = Grid::generateGridVertices(gridSize, gridSpacing);
    g->gridVAO = Grid::setupGridVAO(gridVertices);

    // scene axis
    const auto axisVertices = generateScaledAxes(sceneRadius);
    g->axisVAO = Axis::setupAxisVAO(axisVertices);

    // light BB
    g->lightbboxVAO = BB::setupCubeVAO(BB::cubeVertices, BB::cubeIndices);
std::vector<glm::vec3> transformedLightPositions;

    // CAMERA
    /*    glm::mat4 centerTranslation = glm::translate(glm::mat4(1.0f), -sceneCenter);
        float cameraDistance = sceneRadius * 2.0f; // Adjust as needed for zoom
        std::cout << "cameraDistance " << cameraDistance << " " << std::endl;
        std::cout << "cameraPos" << g->cameraPos.x << " " << g->cameraPos.y << " " << g->cameraPos.z << " " << std::endl;
        g->cameraTarget = glm::vec3(0.0f);
        glm::mat4 view = glm::lookAt(g->cameraPos, g->cameraTarget, g->cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    */

    // Compute the camera position based on the current rotation
   // g->cameraPos = Camera::computeCameraPosition(g->cameraDistance, g->rotationX, g->rotationY, sceneCenter);
   // g->cameraPos=glm::vec3(50,50,50);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    // Update the view matrix
   // glm::mat4 view = glm::lookAt(g->cameraPos, g->cameraTarget, g->cameraUp);

    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::translate(rotationMatrix, -sceneCenter);
    rotationMatrix = glm::translate(rotationMatrix, sceneCenter);

    glm::mat4 centerTranslation = glm::translate(glm::mat4(1.0f), -sceneCenter);
    glm::mat4 sceneTransform = rotationMatrix * centerTranslation;

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


 // Keep the camera at a fixed altitude
        float altitude = g->cameraPos.y; // Use the initial Y position of the camera
        g->cameraPos = Camera::computeHorizontalCameraPosition(g->cameraDistance, g->rotationY, altitude, sceneCenter);

        glm::mat4 view = glm::lookAt(g->cameraPos, g->cameraTarget, g->cameraUp);

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix = glm::translate(rotationMatrix, -sceneCenter);                                         // Move to origin for rotation
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(g->rotationX), glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(g->rotationY), glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis
        rotationMatrix = glm::translate(rotationMatrix, sceneCenter);                                          // Translate back to scene center
        glm::mat4 sceneTransform = rotationMatrix * centerTranslation;

        for (const auto &model : modelDatas)
        {
            // Compute the center of the model's bounding box
            glm::vec3 modelMin, modelMax;
            std::tie(modelMin, modelMax) = loader.computeModelBounds(*model.gltfModel);
            glm::vec3 modelCenter = (modelMin + modelMax) * 0.5f;

            // Compute the translation to center the model
            glm::vec3 translationToCenter = -modelCenter;              // Shift to origin
            translationToCenter.y -= (modelMax.y - modelMin.y) * 0.5f; // Optional: Adjust for base alignment

            /*
              // Apply the translation to the model's base transform
              glm::mat4 centerTransform = glm::translate(glm::mat4(1.0f), translationToCenter);

              // Combine with the model's existing base transform
              glm::mat4 baseTransform = sceneTransform * centerTransform * glm::translate(glm::mat4(1.0f), model.position);
  */
            glm::mat4 centerTransform = glm::translate(glm::mat4(1.0f), translationToCenter);
            glm::mat4 baseTransform = sceneTransform * centerTransform * glm::translate(glm::mat4(1.0f), model.position);

g->transformedLightPositions.clear();
for (const auto& lightPos : renderer.lightPositions) {
    // Transform the light position using the sceneTransform matrix
    glm::vec4 transformedPos = sceneTransform * glm::vec4(lightPos, 1.0f);
    g->transformedLightPositions.push_back(glm::vec3(transformedPos));
}
            // Pass model data and process the corresponding GLTF node
            processNode(g,model, *model.gltfNode, *model.gltfModel, baseTransform, view, projection, g->cameraPos, renderer);
            Axis::renderAxes(model.axisVAO, view, projection, sceneTransform, renderer.axisShader);
        }

        // Render bounding box
        BB::renderSceneBB(g->scenebboxVAO, view, projection, sceneTransform, renderer.bboxShader);
        Axis::renderAxes(g->axisVAO, view, projection, sceneTransform, renderer.axisShader);
        BB::renderLightCubes(g->lightbboxVAO, renderer.lightPositions, view, projection, sceneTransform, renderer.bboxShader);
        Grid::renderGrid(g->gridVAO, view, projection,sceneTransform, renderer.bboxShader,gridSize);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanupOnExit(g, modelDatas);
    return 0;
}
#include "PBRRenderer.h"

#include <tiny_gltf.h>
#include <vector>
#include <set>
#include <string>
#include "Model.h"
#include <memory>

#include "AxisVertex.h"
class ModelLoader
{
public:
    GLuint baseColorTexture = 0;
    std::vector<std::shared_ptr<tinygltf::Model>> gltfModels;
    std::vector<ModelData> models;

    void processNodeToModelData(const tinygltf::Node &node, const std::shared_ptr<tinygltf::Model> &gltfModel, const glm::vec3 &basePosition,
                                std::vector<ModelData> &models, std::set<int> &processedMeshes, const glm::mat4 &parentTransform = glm::mat4(1.0f))
    {
        std::cout << "Node: " << (node.name.empty() ? "Unnamed" : node.name) << std::endl;
        if (!node.translation.empty())
        {
            std::cout << "  Translation: " << node.translation[0] << ", " << node.translation[1] << ", " << node.translation[2] << std::endl;
        }
        if (!node.rotation.empty())
        {
            std::cout << "  Rotation: " << node.rotation[0] << ", " << node.rotation[1] << ", " << node.rotation[2] << ", " << node.rotation[3] << std::endl;
        }
        if (!node.scale.empty())
        {
            std::cout << "  Scale: " << node.scale[0] << ", " << node.scale[1] << ", " << node.scale[2] << std::endl;
        }
        // Compute local transform for the node
        glm::mat4 localTransform = glm::mat4(1.0f);
        if (!node.translation.empty())
        {
            glm::vec3 translation(node.translation[0], node.translation[1], node.translation[2]);
            localTransform = glm::translate(localTransform, translation);
        }
        if (!node.rotation.empty())
        {
            glm::quat rotation(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
            localTransform *= glm::mat4_cast(rotation);
        }
        if (!node.scale.empty())
        {
            glm::vec3 scale(node.scale[0], node.scale[1], node.scale[2]);
            localTransform = glm::scale(localTransform, scale);
        }

        glm::mat4 nodeTransform = parentTransform * localTransform;

        // Process meshes in the node
        if (node.mesh >= 0)
        {
            std::cout << "  Mesh: " << gltfModel->meshes[node.mesh].name << " (Index: " << node.mesh << ")" << std::endl;

            if (processedMeshes.find(node.mesh) == processedMeshes.end())
            {
                const auto &mesh = gltfModel->meshes[node.mesh];
                processedMeshes.insert(node.mesh); // Mark the mesh as processed

                int idx = 0;
                for (size_t i = 0; i < mesh.primitives.size(); ++i)
                {
                    const auto &primitive = mesh.primitives[i];
                    ModelData model;
                    std::cout << "    Primitive " << i << ":" << std::endl;

                    // Check if POSITION attribute exists
                    if (primitive.attributes.find("POSITION") != primitive.attributes.end())
                    {
                        int positionIndex = primitive.attributes.at("POSITION");
                        const auto &accessor = gltfModel->accessors[positionIndex];
                        std::cout << "      POSITION accessor count: " << accessor.count << std::endl;
                        std::cout << "      Min: " << accessor.minValues[0] << ", " << accessor.minValues[1] << ", " << accessor.minValues[2] << std::endl;
                        std::cout << "      Max: " << accessor.maxValues[0] << ", " << accessor.maxValues[1] << ", " << accessor.maxValues[2] << std::endl;
                    }
                    else
                    {
                        std::cout << "      Error: No POSITION attribute found in primitive!" << std::endl;
                    }

                    model.gltfModel = gltfModel;
                    model.gltfNode = &node;
                    model.primitiveIdInGLTFNodeMesh = idx;
                    idx++;

                    // Set base position and transform
                    model.position = basePosition;

                    // Process material (optional)
                    // if (primitive.material >= 0 && primitive.material < gltfModel->materials.size()) {
                    //     const auto& material = gltfModel->materials[primitive.material];
                    //     if (material.pbrMetallicRoughness.baseColorFactor.size() == 4) {
                    //         model.baseColorFactor = glm::vec4(
                    //             material.pbrMetallicRoughness.baseColorFactor[0],
                    //             material.pbrMetallicRoughness.baseColorFactor[1],
                    //             material.pbrMetallicRoughness.baseColorFactor[2],
                    //             material.pbrMetallicRoughness.baseColorFactor[3]
                    //         );
                    //     }
                    //     model.metallicFactor = material.pbrMetallicRoughness.metallicFactor;
                    //     model.roughnessFactor = material.pbrMetallicRoughness.roughnessFactor;
                    // }

                    const auto &material = gltfModel->materials[primitive.material];

                    // Get material properties
                    if (material.values.find("baseColorFactor") != material.values.end())
                    {
                        const auto &color = material.values.at("baseColorFactor").ColorFactor();
                        model.baseColorFactor = glm::vec4(color[0], color[1], color[2], color[3]);
                    }
                    else
                    {
                        model.baseColorFactor = glm::vec4(1.0f);
                    }

                    model.metallicFactor = material.values.count("metallicFactor") ? static_cast<float>(material.values.at("metallicFactor").Factor()) : 1.0f;

                    model.roughnessFactor = material.values.count("roughnessFactor") ? static_cast<float>(material.values.at("roughnessFactor").Factor()) : 1.0f;

                    if (material.values.find("baseColorTexture") != material.values.end())
                    {
                        int textureIndex = material.values.at("baseColorTexture").TextureIndex();
                        const auto &image = gltfModel->images[gltfModel->textures[textureIndex].source];
                        std::cout << "    Texture width: " << image.width << ", height: " << image.height << std::endl;
                        model.baseColorTexture = loadTexture(image);
                    }
                    if (primitive.attributes.find("TEXCOORD_0") == primitive.attributes.end())
                    {
                        std::cerr << "Error: TEXCOORD_0 not found in attributes." << std::endl;
                    }

                    const auto &uvAccessor = gltfModel->accessors[primitive.attributes.at("TEXCOORD_0")];
                    const auto &uvView = gltfModel->bufferViews[uvAccessor.bufferView];
                    const auto &uvBuffer = gltfModel->buffers[uvView.buffer];

                    int uvIndex = primitive.attributes.at("TEXCOORD_0");
                    const auto &uvAccessor1 = gltfModel->accessors[uvIndex];
                    std::cout << "    UV accessor count: " << uvAccessor1.count << std::endl;

                    // Load geometry
                    setupBuffers(model, gltfModel, primitive);

                    // Upload mesh to GPU (VAO, VBO, EBO)
                    // ... (similar to the previous example) ...
 const auto axisVertices = generateScaledAxes(1);
                     model.axisVAO=setupAxisVAO(axisVertices);
                    models.push_back(model);
                }
            }
        }
        else
        {
            std::cout << "  No mesh in this node." << std::endl;
        }
        // Recursively process child nodes
        for (const auto &childIndex : node.children)
        {
            processNodeToModelData(gltfModel->nodes[childIndex], gltfModel, basePosition, models, processedMeshes, nodeTransform);
        }
    }
    std::pair<glm::vec3, glm::vec3> computeModelBounds(const tinygltf::Model &model)
    {
        glm::vec3 min(FLT_MAX), max(-FLT_MAX);

        for (const auto &mesh : model.meshes)
        {
            for (const auto &primitive : mesh.primitives)
            {
                if (primitive.attributes.find("POSITION") == primitive.attributes.end())
                {
                    continue; // Skip if no POSITION attribute
                }

                const auto &accessor = model.accessors[primitive.attributes.at("POSITION")];
                const auto &bufferView = model.bufferViews[accessor.bufferView];
                const auto &buffer = model.buffers[bufferView.buffer];

                const float *positions = reinterpret_cast<const float *>(
                    &buffer.data[bufferView.byteOffset + accessor.byteOffset]);

                for (size_t i = 0; i < accessor.count; ++i)
                {
                    glm::vec3 vertex(
                        positions[i * 3 + 0],
                        positions[i * 3 + 1],
                        positions[i * 3 + 2]);

                    min = glm::min(min, vertex);
                    max = glm::max(max, vertex);
                }
            }
        }

        return {min, max};
    }
    std::pair<glm::vec3, glm::vec3> computeSceneBounds(const std::vector<std::shared_ptr<tinygltf::Model>> &models)
    {
        glm::vec3 sceneMin(FLT_MAX), sceneMax(-FLT_MAX);

        for (const auto &model : models)
        {
            auto [modelMin, modelMax] = computeModelBounds(*model);
            sceneMin = glm::min(sceneMin, modelMin);
            sceneMax = glm::max(sceneMax, modelMax);
        }

        return {sceneMin, sceneMax};
    }

    glm::vec3 computeSceneCenter(const std::vector<std::shared_ptr<tinygltf::Model>> &models)
    {
        auto [sceneMin, sceneMax] = computeSceneBounds(models);
        std::cout << "Scene min " << sceneMin.x << " " << sceneMin.y << " " << sceneMin.z << std::endl;
        std::cout << "Scene max " << sceneMax.x << " " << sceneMax.y << " " << sceneMax.z << std::endl;
        float sceneRadius = glm::length(sceneMax - sceneMin) * 0.5f;
        std::cout << "Scene radius " << sceneRadius << std::endl;
        return (sceneMin + sceneMax) * 0.5f; // Center of the bounding box
    }

    GLuint setupAxisVAO(const std::vector<AxisVertex> &vertices)
    {
        GLuint VAO, VBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(AxisVertex), vertices.data(), GL_STATIC_DRAW);

        // Position attribute (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AxisVertex), (void *)offsetof(AxisVertex, position));
        glEnableVertexAttribArray(0);

        // Color attribute (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AxisVertex), (void *)offsetof(AxisVertex, color));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        return VAO;
    }
    void loadModel(tinygltf::TinyGLTF &loader, const std::string path, const glm::vec3 position)
    {
        std::cout << "Load model" << path << std::endl;

        std::string err, warn;
        // Assign GLTF model and node references
        std::shared_ptr<tinygltf::Model> gltfModel = std::make_shared<tinygltf::Model>();

        if (!loader.LoadBinaryFromFile(gltfModel.get(), &err, &warn, path))
        {
            std::cerr << "Failed to load " << path << ": " << err << std::endl;
        }
        gltfModels.push_back(gltfModel);
        std::cout << "GLTF loaded" << path << std::endl;

        std::set<int> processedMeshes;
        // Process only top-level nodes referenced by scenes
        for (const auto &scene : gltfModel->scenes)
        {
            for (const auto &nodeIndex : scene.nodes)
            {
                std::cout << "Node " << nodeIndex << std::endl;
                processNodeToModelData(gltfModel->nodes[nodeIndex], gltfModel, position, models, processedMeshes);
            }
        }
    }
    std::vector<ModelData> loadModels(const std::vector<std::pair<std::string, glm::vec3>> &modelPaths)
    {
        models.clear();
        // std::vector<Model> models;
        tinygltf::TinyGLTF loader;

        for (const auto &[path, position] : modelPaths)
        {
            loadModel(loader, path, position);
        }
        return models;
    }

    GLuint loadTexture(const tinygltf::Image &image)
    {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_SRGB,
            //            GL_RGBA,
            image.width,
            image.height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            image.image.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);
        return texture;
    }

    void printModelsInfo() const
    {
        std::cout << "Number of models: " << models.size() << std::endl;

        for (size_t i = 0; i < models.size(); i++)
        {
            std::cout << " model " << i << std::endl;

            if (models[i].gltfModel == nullptr)
            {
                std::cerr << "Error: No associated GLTF model for model " << i << std::endl;
                continue;
            }

            const auto &gltfModel = *(models[i].gltfModel);

            const auto &gltfNode = *(models[i].gltfNode);

            const auto &mesh = gltfNode.mesh;
        }
    }

private:
    void setupBuffers(ModelData &model, const std::shared_ptr<tinygltf::Model> gltfModel, const tinygltf::Primitive &primitive)
    {
        const auto &posAccessor = gltfModel->accessors[primitive.attributes.at("POSITION")];
        const auto &posView = gltfModel->bufferViews[posAccessor.bufferView];
        const auto &posBuffer = gltfModel->buffers[posView.buffer];

        const auto &normalAccessor = gltfModel->accessors[primitive.attributes.at("NORMAL")];
        const auto &normalView = gltfModel->bufferViews[normalAccessor.bufferView];
        const auto &normalBuffer = gltfModel->buffers[normalView.buffer];

        const auto &indexAccessor = gltfModel->accessors[primitive.indices];
        const auto &indexView = gltfModel->bufferViews[indexAccessor.bufferView];
        const auto &indexBuffer = gltfModel->buffers[indexView.buffer];

        std::vector<float> vertexData;
        const float *positions = reinterpret_cast<const float *>(&posBuffer.data[posView.byteOffset]);
        const float *normals = reinterpret_cast<const float *>(&normalBuffer.data[normalView.byteOffset]);

        const auto &uvAccessor = gltfModel->accessors[primitive.attributes.at("TEXCOORD_0")];
        const auto &uvView = gltfModel->bufferViews[uvAccessor.bufferView];
        const auto &uvBuffer = gltfModel->buffers[uvView.buffer];

        const float *uvs = reinterpret_cast<const float *>(&uvBuffer.data[uvView.byteOffset]);

        for (size_t i = 0; i < posAccessor.count; i++)
        {
            vertexData.insert(vertexData.end(), {
                                                    positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2], // Position
                                                    normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2],       // Normal
                                                    uvs[i * 2], uvs[i * 2 + 1]                                    // UV
                                                });
        }
        glGenVertexArrays(1, &model.VAO);
        glGenBuffers(1, &model.VBO);
        glGenBuffers(1, &model.EBO);

        glBindVertexArray(model.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexAccessor.count * sizeof(uint16_t),
                     &indexBuffer.data[indexView.byteOffset], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        model.indexCount = indexAccessor.count;
    }
};

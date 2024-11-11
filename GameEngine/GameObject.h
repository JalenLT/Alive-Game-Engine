#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Observer.h"
#include "Material.h"
#include "Event.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>
#include "glm_serialization.h"
#include <memory>
#include <filesystem>

class GameObject : public Observer {
public:
    // OpenGL handles - Do not serialize
    unsigned int VAO, VBO, EBO;

    std::string name;
    std::string path = "tmp";
    std::weak_ptr<GameObject> parent; // Changed from raw pointer to std::weak_ptr
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    Material material;

    GameObject() : VAO(0), VBO(0), EBO(0), name("Cube") {};
    ~GameObject() = default;

    void update(const EventData& data) override {}

    void initialize(const std::string& path, std::shared_ptr<GameObject> parent = nullptr) {
        this->path = path;
        this->parent = parent;

        loadModel(path);
    }

    void loadModel(const std::string& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "Error from ASSIMP: " << importer.GetErrorString() << std::endl;
            return;
        }

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    void processMesh(aiMesh* mesh, const aiScene* scene) {
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            vertices.push_back(mesh->mVertices[i].x);
            vertices.push_back(mesh->mVertices[i].y);
            vertices.push_back(mesh->mVertices[i].z);

            if (mesh->mNormals) {
                vertices.push_back(mesh->mNormals[i].x);
                vertices.push_back(mesh->mNormals[i].y);
                vertices.push_back(mesh->mNormals[i].z);
            }
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        // OpenGL buffer setup (not serialized)
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    template <class Archive>
    void serialize(Archive& ar) {
        ar(
            CEREAL_NVP(name),
            CEREAL_NVP(path),
            CEREAL_NVP(modelMatrix),
            CEREAL_NVP(material)
            // parent not serialized, would require handling circular references
        );
    }
};

#endif
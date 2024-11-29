#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Observer.h"
#include "Material.h"
#include "Event.h"
#include "Transform.h"
#include "assimp/scene.h"
#include "Mesh.h"
#include "BoundingBox.h"
#include "Tags.h"
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>
#include "glm_serialization.h"
#include <memory>
#include <vector>
#include <string>
#include <iostream>

class GameObject : public Observer {
public:
    int id;
    std::string name;
    std::string path;
    std::weak_ptr<GameObject> parent; // Changed from raw pointer to std::weak_ptr
    std::vector<std::weak_ptr<GameObject>> children;
    Transform transform;
    Mesh mesh;
    BoundingBox boundingBox;
    std::vector<Tags> tags;

    GameObject();
    ~GameObject();

    void update(const EventData& data) override;

    void initialize(const int id, const std::string& path, std::shared_ptr<GameObject> parent = nullptr);

    void initialize(const int id, const Mesh mesh, const Transform& transform, std::shared_ptr<GameObject> parent = nullptr);

    void loadModel(const std::string& path);

    void processNode(aiNode* node, const aiScene* scene);

    void processMesh(aiMesh* mesh, const aiScene* scene);

    bool containsTag(const std::vector<Tags>& tags, Tags tag);

    template <class Archive>
    void serialize(Archive& ar) {
        ar(
            CEREAL_NVP(id),
            CEREAL_NVP(name),
            CEREAL_NVP(path),
            CEREAL_NVP(transform),
            CEREAL_NVP(mesh),
            CEREAL_NVP(boundingBox)
        );
    }
};

#endif
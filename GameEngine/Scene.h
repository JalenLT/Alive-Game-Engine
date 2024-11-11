#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory> // For std::unique_ptr and smart pointers
#include "serialization_utils.h"
#include "GameObject.h"
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>

class Scene {
public:
    Scene() = default;
    ~Scene() = default;

    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::string name;

    void addGameObject(const std::string& path, const std::string& name) {
        auto gameObject = std::make_shared<GameObject>();
        gameObject->name = name;
        gameObject->initialize(path);
        gameObjects.push_back(std::move(gameObject));
    }

    template <class Archive>
    void save(Archive& ar) const {
        ar(CEREAL_NVP(name));

        // Flatten gameObjects to plain vector of GameObject
        std::vector<GameObject> gameObjects;
        for (const auto& gameObject : this->gameObjects) {
            if (gameObject) {
                gameObjects.push_back(*gameObject); // Dereference shared_ptr to get plain GameObject
            }
        }

        ar(CEREAL_NVP(gameObjects));
    }

    template <class Archive>
    void load(Archive& ar) {
        ar(CEREAL_NVP(name));

        // Load into a plain vector of GameObject
        std::vector<GameObject> gameObjects;
        ar(CEREAL_NVP(gameObjects));
        std::cout << gameObjects[0].path << std::endl;

        // Reconstruct the shared pointers from the plain objects
        this->gameObjects.clear();
        for (const auto& obj : gameObjects) {
            addGameObject(obj.path, obj.name);
            this->gameObjects.back()->modelMatrix = obj.modelMatrix;
        }
    }
};

#endif

#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory> // For std::unique_ptr and smart pointers
#include "serialization_utils.h"
#include "GameObject.h"
#include "Light.h"
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>

class Scene {
public:
    Scene() = default;
    ~Scene() = default;

    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::vector<std::shared_ptr<Light>> lights;
    std::string name;

    void addGameObject(const std::string& path, const std::string& name) {
        auto gameObject = std::make_shared<GameObject>();
        gameObject->name = name;
        gameObject->initialize(gameObjects.size(), path);
        gameObjects.push_back(std::move(gameObject));
    }

    void addLight(const std::string& type, glm::vec3 position) {
        auto light = std::make_shared<Light>();
        light->id = lights.size();
        light->type = type;
        light->transform.position = position;
        lights.push_back(std::move(light));
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

        // Flatten lights to plain vector of Light
        std::vector<Light> lights;
        for (const auto& light : this->lights) {
            if (light) {
                lights.push_back(*light); // Dereference shared_ptr to get plain Light
            }
        }

        ar(CEREAL_NVP(gameObjects));
        ar(CEREAL_NVP(lights));
    }

    template <class Archive>
    void load(Archive& ar) {
        ar(CEREAL_NVP(name));

        // Load into a plain vector of GameObject
        std::vector<GameObject> gameObjects;
        ar(CEREAL_NVP(gameObjects));

        // Load into a plain vector of Light
        std::vector<Light> lights;
        ar(CEREAL_NVP(lights));

        // Reconstruct the shared pointers from the plain objects
        this->gameObjects.clear();
        this->lights.clear();
        for (const auto& obj : gameObjects) {
            addGameObject(obj.path, obj.name);
            this->gameObjects.back()->transform = obj.transform;
        }
        for (const auto& obj : lights) {
            addLight(obj.type, obj.transform.position);
            this->lights.back()->material = obj.material;
        }
    }
};

#endif

#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <string>
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>
#include "GameObject.h"
#include "Light.h"
#include "serialization_utils.h"

class Scene {
public:
    Scene();
    ~Scene();

    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::vector<std::shared_ptr<Light>> lights;
    std::string name;

    void addGameObject(const std::string& path, const std::string& name);

    void addGameObject(const GameObject& gameObject);

    void addLight(const std::string& type, glm::vec3 position);

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
        for (const GameObject& obj : gameObjects) {
            addGameObject(obj);
        }
        for (const auto& obj : lights) {
            addLight(obj.type, obj.transform.position);
            this->lights.back()->material = obj.material;
        }
    }
};

#endif

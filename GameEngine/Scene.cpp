#include "Scene.h"

Scene::Scene() = default;
Scene::~Scene() = default;

std::vector<std::shared_ptr<GameObject>> gameObjects;
std::vector<std::shared_ptr<Light>> lights;
std::string name;

void Scene::addGameObject(const std::string& path, const std::string& name) {
    auto gameObject = std::make_shared<GameObject>();
    gameObject->name = name;
    gameObject->initialize(gameObjects.size(), path);
    gameObjects.push_back(std::move(gameObject));
}

void Scene::addLight(const std::string& type, glm::vec3 position) {
    auto light = std::make_shared<Light>();
    light->id = lights.size();
    light->type = type;
    light->transform.position = position;
    lights.push_back(std::move(light));
}
#include "SceneManager.h"
#include "serialization_utils.h"

SceneManager& SceneManager::getInstance() {
	static SceneManager instance;
	return instance;
}

void SceneManager::initialize() {
	currentScene = std::make_shared<Scene>();
}

void SceneManager::loadSceneFromFile(const std::string& filename) {
	currentScene = std::make_shared<Scene>(loadFromFile<Scene>(filename, "Scene"));
}

SceneManager::SceneManager() : currentScene(nullptr) {}
SceneManager::~SceneManager() {}
void SceneManager::setCurrentScene(std::shared_ptr<Scene> scene) { currentScene = scene; }
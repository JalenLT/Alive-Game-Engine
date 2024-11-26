#include "SceneManager.h"
#include "serialization_utils.h"
#include "tinyfiledialogs/tinyfiledialogs.h"
#include <filesystem>

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

void SceneManager::update(const EventData& data) {
	if (data.type == EventType::AddGameObject) {
		std::cout << "test" << std::endl;
		const char* filePath = tinyfd_openFileDialog(
			"Select a File",
			"",
			0,
			NULL,
			NULL,
			0
		);

		if (filePath) {
			std::string fullPath(filePath);

			std::string fileName = std::filesystem::path(fullPath).filename().string();

			GameObject gameObject;
			gameObject.id = this->currentScene->gameObjects.size();
			gameObject.name = fileName;
			gameObject.loadModel(std::string(filePath));
			this->currentScene->addGameObject(gameObject);
		}
	}
}

SceneManager::SceneManager() : currentScene(nullptr) {}
SceneManager::~SceneManager() {}
void SceneManager::setCurrentScene(std::shared_ptr<Scene> scene) { currentScene = scene; }
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

			this->currentScene->addGameObject(fullPath, fileName);

			EventData data{ EventType::RefreshSceneHierarchy };

			EventManager::getInstance().notifyObservers(data);
		}
	}
	else if (data.type == EventType::UpdateTransform) {
		if(data.gameObjectIndex.has_value()) {
			std::shared_ptr<GameObject> gameObject = this->currentScene->gameObjects[data.gameObjectIndex.value()];
			if (data.position.has_value()) {
				gameObject->transform.translateGlobal(data.position.value());
				if (gameObject->children.size() > 0) {
					std::cout << "Has Children" << std::endl;
					for (auto& child : gameObject->children) {
						child.lock()->transform.translateGlobal(data.position.value());
					}
				}
			}
			else if (data.rotation.has_value()) {
				gameObject->transform.setGlobalRotation(data.rotation.value());

			}
		}
	}
}

SceneManager::SceneManager() : currentScene(nullptr) {}
SceneManager::~SceneManager() {}
void SceneManager::setCurrentScene(std::shared_ptr<Scene> scene) { currentScene = scene; }
#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include <memory>
#include "serialization_utils.h"

class SceneManager {
public:
	std::shared_ptr<Scene> currentScene;

	static SceneManager& getInstance() {
		static SceneManager instance;
		return instance;
	}

	void initialize() {
		currentScene = std::make_shared<Scene>();
	}

	void loadSceneFromFile(const std::string& filename) {
		currentScene = std::make_shared<Scene>(loadFromFile<Scene>(filename, "Scene"));
	}

private:
	SceneManager() : currentScene(nullptr) {}
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	~SceneManager() {}
	void setCurrentScene(std::shared_ptr<Scene> scene) { currentScene = scene; }
};

#endif

#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include <memory>
#include <string>
#include "serialization_utils.h"

class SceneManager {
public:
	std::shared_ptr<Scene> currentScene;

	static SceneManager& getInstance();

	void initialize();

	void loadSceneFromFile(const std::string& filename);

private:
	SceneManager();
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	~SceneManager();
	void setCurrentScene(std::shared_ptr<Scene> scene);
};

#endif

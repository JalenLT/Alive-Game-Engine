#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "Observer.h"
#include "EventManager.h"
#include <memory>
#include <string>
#include "serialization_utils.h"

class SceneManager: public Observer {
public:
	std::shared_ptr<Scene> currentScene;

	static SceneManager& getInstance();

	void initialize();

	void loadSceneFromFile(const std::string& filename);

	void update(const EventData& data) override;

private:
	SceneManager();
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	~SceneManager();
	void setCurrentScene(std::shared_ptr<Scene> scene);
};

#endif

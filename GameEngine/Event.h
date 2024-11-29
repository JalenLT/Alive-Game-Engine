#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <optional>
#include <string>
#include <filesystem>
#include <memory>
#include <vector>
#include <array>
#include "glm/mat4x4.hpp"

enum class EventType {
	FileSelected,
	RefreshSceneHierarchy,
	GameObjectSelected,
	LightSelected,
	MouseLeftClicked,
	MouseRightClicked,
	AddGameObject,
	AddLight,
	UpdateTransform
};

struct EventData {
	EventType type;
	std::optional<std::filesystem::path> filePath;
	std::optional<int> gameObjectIndex;
	std::optional<int> lightIndex;
	std::optional<std::vector<double>> mousePosition;
	std::optional<glm::vec3> position;
	std::optional<glm::vec3> rotation;
	std::optional<glm::vec3> scale;

	EventData(EventType eventType);
};

#endif

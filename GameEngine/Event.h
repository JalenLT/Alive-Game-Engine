#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <optional>
#include <string>
#include <filesystem>
#include <memory>
#include <vector>
#include "glm/mat4x4.hpp"

enum class EventType {
	FileSelected,
	RefreshSceneHierarchy,
	GameObjectSelected,
	LightSelected,
	MouseClicked,
};

struct EventData {
	EventType type;
	std::optional<std::filesystem::path> filePath;
	std::optional<int> gameObjectIndex;
	std::optional<int> lightIndex;
	std::optional<std::vector<double>> mousePosition;

	EventData(EventType eventType) : type(eventType) {};
};

#endif

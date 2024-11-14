#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <optional>
#include <string>
#include <filesystem>
#include <memory>
#include "glm/mat4x4.hpp"

enum class EventType {
	FileSelected,
	RefreshSceneHierarchy,
	GameObjectSelected,
	LightSelected
};

struct EventData {
	EventType type;
	std::optional<std::filesystem::path> filePath;
	std::optional<int> gameObjectIndex;
	std::optional<int> lightIndex;

	EventData(EventType eventType) : type(eventType) {};
};

#endif

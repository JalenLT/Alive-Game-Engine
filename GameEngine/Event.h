#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <optional>
#include <string>
#include <filesystem>
#include <memory>

enum class EventType {
	FileSelected,
	RefreshSceneHierarchy
};

struct EventData {
	EventType type;
	std::optional<std::filesystem::path> filePath;

	EventData(EventType eventType) : type(eventType) {};
};

#endif

#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <optional>
#include <string>
#include <filesystem>

enum class EventType {
	FileSelected
};

struct EventData {
	EventType type;
	std::optional<std::filesystem::path> filePath;

	EventData(EventType eventType) : type(eventType) {};
};

#endif

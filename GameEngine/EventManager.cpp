#include "EventManager.h"
#include <iostream>

EventManager::EventManager() {}

EventManager::~EventManager() {}

EventManager& EventManager::getInstance() {
	static EventManager instance;
	return instance;
}

void EventManager::addObserver(Observer* observer) {
	observers.push_back(observer);
}

void EventManager::removeObserver(Observer* observer) {
	observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void EventManager::notifyObservers(const EventData& data) {
	for (auto* observer : observers) {
		observer->update(data);
	}
}

void EventManager::triggerEvent(const EventData& data) {
	notifyObservers(data);
}
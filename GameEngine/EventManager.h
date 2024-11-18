#pragma once
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Subject.h"
#include <vector>
#include <algorithm>

class EventManager : public Subject {
private:
	std::vector<Observer*> observers;

	EventManager();
	EventManager(const EventManager&) = delete;
	EventManager& operator=(const EventManager&) = delete;
	~EventManager();

public:
	static EventManager& getInstance();
	void addObserver(Observer* observer) override;
	void removeObserver(Observer* observer) override;
	void notifyObservers(const EventData& data) override;
	void triggerEvent(const EventData& data);
};

#endif
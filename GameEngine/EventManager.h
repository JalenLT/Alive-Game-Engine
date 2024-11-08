#pragma once
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Subject.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>



class EventManager : public Subject {
private:
	std::vector<Observer*> observers;

	EventManager() {}
	EventManager(const EventManager&) = delete;
	EventManager& operator=(const EventManager&) = delete;
	~EventManager() {}

public:
	static EventManager& getInstance() {
		static EventManager instance;
		return instance;
	}

	void addObserver(Observer* observer) override {
		observers.push_back(observer);
	}

	void removeObserver(Observer* observer) override {
		observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
	}

	void notifyObservers(const std::string& event) override {
		for (auto* observer : observers) {
			observer->update(event);
		}
	}

	void triggerEvent(const std::string& event) {
		std::cout << "Event triggered" << event << std::endl;
		notifyObservers(event);
	}
};

#endif
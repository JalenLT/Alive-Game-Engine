#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"
#include "Event.h"

class Subject {
public:
	virtual void addObserver(Observer* observer) = 0;
	virtual void removeObserver(Observer* observer) = 0;
	virtual void notifyObservers(const EventData& data) = 0;
	virtual ~Subject() = default;
};

#endif
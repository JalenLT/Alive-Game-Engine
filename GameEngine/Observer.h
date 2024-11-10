#pragma once
#ifndef OBSERVER_H
#define OBSERVER_H

#include <iostream>
#include <vector>
#include <string>
#include "Event.h"

class Observer {
public:
	virtual void update(const EventData& data) = 0;
	virtual ~Observer() = default;
};

#endif
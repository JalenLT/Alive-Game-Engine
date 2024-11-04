#pragma once
#ifndef OBSERVER_H
#define OBSERVER_H

#include <iostream>
#include <vector>
#include <string>

class Observer {
public:
	virtual void update(const std::string& event) = 0;
};

#endif
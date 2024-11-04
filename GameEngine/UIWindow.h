#pragma once
#ifndef UIWINDOW_H
#define UIWINDOW_H

#include "imgui.h"
#include "Window.h"

class UIWindow {
public:
	float x, y, width, height;

	UIWindow(float x = 0.0f, float y = 0.0f, float width = 300.0f, float height = 400.0f) : x(x), y(y), width(width), height(height) {};
	virtual ~UIWindow() = default;
	virtual void initialize() {};
	virtual void render() = 0;
};

#endif

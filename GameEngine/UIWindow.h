#pragma once
#ifndef UIWINDOW_H
#define UIWINDOW_H

#include "imgui.h"
#include "Window.h"

class UIWindow {
public:
	virtual ~UIWindow() = default;
	virtual void initialize() {};
	virtual void render() = 0;
};

#endif

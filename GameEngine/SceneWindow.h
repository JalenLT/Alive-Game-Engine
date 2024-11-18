#pragma once
#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include "UIWindow.h"
#include "Observer.h"
#include "Event.h"
#include <iostream>

class SceneWindow : public UIWindow, public Observer {
public:
	SceneWindow(float x = 0.0f, float y = 0.0f, float width = 300.0f, float height = 400.0f);

	void update(const EventData& data) override;

	void render() override;
};

#endif
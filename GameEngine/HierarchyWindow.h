#pragma once
#ifndef HIERARCHYWINDOW_H
#define HIERARCHYWINDOW_H

#include "UIWindow.h"
#include "Observer.h"
#include "SceneManager.h"
#include "EventManager.h"
#include <functional>
#include <memory>

class HierarchyWindow : public UIWindow, public Observer {
public:
	std::shared_ptr<Scene> scene = nullptr;

    HierarchyWindow(float x = 0.0f, float y = 0.0f, float width = 300.0f, float height = 400.0f);

    void update(const EventData& data) override;
	
	void render() override;

private:
	std::function<void()> callback = nullptr;
};

#endif
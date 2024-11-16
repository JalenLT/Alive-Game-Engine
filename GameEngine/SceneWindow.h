#pragma once
#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include "UIWindow.h"
#include "Event.h"

class SceneWindow : public UIWindow, public Observer {
public:
	SceneWindow(float x = 0.0f, float y = 0.0f, float width = 300.0f, float height = 400.0f) : UIWindow(x, y, width, height) {}

	void update(const EventData& data) override {}

	void render() override {
		int windowWidth, windowHeight;
		glfwGetWindowSize(Window::getInstance().getWindow(), &windowWidth, &windowHeight);

		setupRender();

		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

		teardownRender();

		glViewport(x, Window::getInstance().getWindowHeight() - y - height, width, height);
	}
};

#endif
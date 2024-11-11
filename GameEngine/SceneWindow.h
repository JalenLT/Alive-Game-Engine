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

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

		ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(width, height));

		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

		ImGui::End();

		ImGui::PopStyleColor(2);

		glViewport(x, Window::getInstance().getWindowHeight() - y - height, width, height);
	}
};

#endif
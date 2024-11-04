#pragma once
#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include "UIWindow.h"

class SceneWindow : public UIWindow, public Observer {
public:
	void update(const std::string& event) override {
		std::cout << "Event: " << event << std::endl;
	}

	void render() override {
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

		ImGui::SetNextWindowPos(ImVec2(Window::getInstance().getMode()->width * 0.2, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(Window::getInstance().getMode()->width * 0.6, Window::getInstance().getMode()->height * 0.65));
		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
		ImGui::Text("This is a test");
		if (ImGui::Button("Event Test")) {
			EventManager::getInstance().notifyObservers("button clicked in scene window");
		}
		ImGui::End();

		ImGui::PopStyleColor(2);
	}
};

#endif
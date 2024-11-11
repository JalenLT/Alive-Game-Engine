#pragma once
#ifndef INSPECTORWINDOW_H
#define INSPECTORWINDOW_H

#include "UIWindow.h"
#include "Observer.h"

class InspectorWindow : public UIWindow, public Observer {
public:
	InspectorWindow(float x = 0.0f, float y = 0.0f, float width = 300.0f, float height = 400.0f) : UIWindow(x, y, width, height) {};

	void update(const EventData& data) override {
		if (data.type == EventType::FileSelected) {
			callback = [&]() {
				ImGui::Text("GameObject");
				ImGui::Text("Path: ");
				ImGui::SameLine();
				if (data.filePath) {
					std::string pathStr = data.filePath->string();
					ImGui::Text(pathStr.c_str());
				}
				else {
					ImGui::Text("No file path available.");
				}

			};
		}
	}
	
	void render() override {
		int windowWidth, windowHeight;
		glfwGetWindowSize(Window::getInstance().getWindow(), &windowWidth, &windowHeight);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

		ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(width, height));
		ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		if (callback != nullptr) {
			callback();
		}
		else {
			ImGui::Text("...");
		}

		ImGui::End();

		ImGui::PopStyleColor(2);
	}

private:
	std::function<void()> callback = nullptr;
};

#endif
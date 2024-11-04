#pragma once
#ifndef FILEBROWSERWINDOW_H
#define FILEBROWSERWINDOW_H

#include "UIWindow.h"

class FileBrowserWindow : public UIWindow {
public:
	void render() override {
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

		ImGui::SetNextWindowPos(ImVec2(0, Window::getInstance().getMode()->height * 0.65), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(Window::getInstance().getMode()->width * 0.8, Window::getInstance().getMode()->width * 0.35));
		ImGui::Begin("File Browser", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::Text("This is a test");
		ImGui::End();

		ImGui::PopStyleColor(2);
	}
};

#endif
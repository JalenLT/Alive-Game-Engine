#pragma once
#ifndef UIWINDOW_H
#define UIWINDOW_H

#include "imgui.h"
#include "Window.h"
#include "functional"

class UIWindow {
public:
	float x, y, width, height;

	UIWindow(float x = 0.0f, float y = 0.0f, float width = 300.0f, float height = 400.0f) : x(x), y(y), width(width), height(height) {};
	virtual ~UIWindow() = default;
	virtual void initialize() {};
	virtual void render() = 0;

	void setupRender() {
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.086f, 0.098f, 0.145f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.056f, 0.038f, 0.015f, 1.0f));

		ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(width, height));
	}

	void teardownRender() {
		ImGui::End();

		ImGui::PopStyleColor(2);
	}
};

#endif

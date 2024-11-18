#include "UIWindow.h"

UIWindow::UIWindow(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {};

void UIWindow::initialize() {};

void UIWindow::setupRender() {
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.086f, 0.098f, 0.145f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.056f, 0.038f, 0.015f, 1.0f));

	ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(width, height));
}

void UIWindow::teardownRender() {
	ImGui::End();

	ImGui::PopStyleColor(2);
}
#pragma once
#ifndef FILEBROWSERWINDOW_H
#define FILEBROWSERWINDOW_H

#include "UIWindow.h"
#include <cstdlib>
#include <filesystem>
namespace fs = std::filesystem;

class FileBrowserWindow : public UIWindow {
public:
	FileBrowserWindow(float x = 0.0f, float y = 0.0f, float width = 300.0f, float height = 400.0f) : UIWindow(x, y, width, height) {};

	void createFolder(const std::string& path) {
		try {
			if (fs::create_directory(path)) {
				std::cout << "Folder Created: " << path << std::endl;
			}
			else {
				std::cout << "Folder already created: " << path << std::endl;
			}
		}
		catch (const fs::filesystem_error& e) {
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
	
	void render() override {
		int windowWidth, windowHeight;
		glfwGetWindowSize(Window::getInstance().getWindow(), &windowWidth, &windowHeight);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

		ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(width, height));
		ImGui::Begin("File Browser", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::Text("This is a test");
		ImGui::End();

		ImGui::PopStyleColor(2);

		createFolder("C:\\Users\\sseunarine\\AppData\\Roaming");
	}
};

#endif
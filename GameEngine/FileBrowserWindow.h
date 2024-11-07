#pragma once
#ifndef FILEBROWSERWINDOW_H
#define FILEBROWSERWINDOW_H

#include "UIWindow.h"
#include <cstdlib>
#include <filesystem>
namespace fs = std::filesystem;

class FileBrowserWindow : public UIWindow {
public:
	FileBrowserWindow(float x = 0.0f, float y = 0.0f, float width = 300.0f, float height = 400.0f) : UIWindow(x, y, width, height){
		createFolder(basePath + "\\GameObjects");
		createFolder(basePath + "\\Materials");
	};

	fs::directory_iterator getDirectoryIterator(const fs::path& directory) {
		return fs::directory_iterator(directory);
	}

	void createFolder(const fs::path& path) {
		try {
			if (fs::create_directories(path)) {
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

	void drawFolder(const char* name, const char* tooltip) {
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 p = ImGui::GetCursorScreenPos();

		ImVec2 folderTop = ImVec2(p.x, p.y);
		drawList->AddRectFilled(ImVec2(p.x, p.y + 10.0f), ImVec2(p.x + 100, p.y + 70), IM_COL32(64, 110, 142, 255)); // Main folder body
		drawList->AddRectFilled(folderTop, ImVec2(folderTop.x + 50, folderTop.y + 20), IM_COL32(35, 57, 91, 255)); // Folder tab

		if (ImGui::InvisibleButton("folder_btn", ImVec2(100, 70))) {
			std::cout << "Folder button clicked!" << std::endl;
		}

		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip(tooltip);
		}
		ImVec2 textPos = ImVec2(p.x, p.y + 75);
		drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), name);
	}
	
	void render() override {
		int windowWidth, windowHeight;
		glfwGetWindowSize(Window::getInstance().getWindow(), &windowWidth, &windowHeight);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

		ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(width, height));
		ImGui::Begin("File Browser", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		for (const auto& item : getDirectoryIterator(basePath)) {
			drawFolder(item.path().filename().string().c_str(), "Open Folder");
			if (ImGui::GetCursorScreenPos().x < (width - 150)) {
				ImGui::SameLine();
			}
			else {
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
			}
		}
		
		ImGui::End();

		ImGui::PopStyleColor(2);
	}

private:
	std::string basePath = "C:\\Users\\USER\\AppData\\Roaming\\Alive";
	std::string currentPath = "C:\\Users\\USER\\AppData\\Roaming\\Alive";
};

#endif
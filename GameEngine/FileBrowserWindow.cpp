#include "FileBrowserWindow.h"
#include "EventManager.h"
#include <iostream>
#include "Window.h"
#include "project_constants.h"

FileBrowserWindow::FileBrowserWindow(float x, float y, float width, float height) : UIWindow(x, y, width, height), basePath(ROAMING_PATH + "\\" + PROJECT_NAME), currentPath(ROAMING_PATH + "\\" + PROJECT_NAME) {
	createFolder(basePath + "\\GameObjects");
	createFolder(basePath + "\\Materials");
	createFolder(basePath + "\\Models");
	createFolder(basePath + "\\Shaders");
};

fs::directory_iterator FileBrowserWindow::getDirectoryIterator(const fs::path& directory) {
	return fs::directory_iterator(directory);
}

void FileBrowserWindow::createFolder(const fs::path& path) {
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

void FileBrowserWindow::addFolder(const std::string name, const char* tooltip, bool goBack) {
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 p = ImGui::GetCursorScreenPos();

	ImVec2 folderTop = ImVec2(p.x, p.y);
	drawList->AddRectFilled(ImVec2(p.x, p.y + 10.0f), ImVec2(p.x + 100, p.y + 70), IM_COL32(64, 110, 142, 255));
	drawList->AddRectFilled(folderTop, ImVec2(folderTop.x + 50, folderTop.y + 20), IM_COL32(35, 57, 91, 255));

	if (ImGui::InvisibleButton(name.c_str(), ImVec2(100, 70))) {
		if (goBack) {
			size_t pos = currentPath.find_last_of("\\/");
			if (pos != std::string::npos) {
				currentPath = currentPath.substr(0, pos);
			}
		}
		else {
			currentPath += "\\" + name;
		}
	}

	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip(tooltip);
	}
	ImVec2 textPos = ImVec2(p.x, p.y + 75);
	drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), name.c_str());
}

void FileBrowserWindow::addFile(const std::string name, const std::string ext) {
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 p = ImGui::GetCursorScreenPos();

	if (ext == ".obj") {
		drawList->AddRectFilled(ImVec2(p.x, p.y + 10.0f), ImVec2(p.x + 100, p.y + 70), IM_COL32(142, 168, 195, 255));

		drawList->AddQuadFilled(ImVec2(p.x + 25, p.y + 30), ImVec2(p.x + 50, p.y + 40), ImVec2(p.x + 50, p.y + 65), ImVec2(p.x + 25, p.y + 55), IM_COL32(22, 25, 37, 255));
		drawList->AddLine(ImVec2(p.x + 25, p.y + 30), ImVec2(p.x + 50, p.y + 20), IM_COL32(22, 25, 37, 255));
		drawList->AddLine(ImVec2(p.x + 50, p.y + 40), ImVec2(p.x + 75, p.y + 30), IM_COL32(22, 25, 37, 255));
		drawList->AddLine(ImVec2(p.x + 50, p.y + 64), ImVec2(p.x + 75, p.y + 55), IM_COL32(22, 25, 37, 255));
		drawList->AddLine(ImVec2(p.x + 75, p.y + 55), ImVec2(p.x + 75, p.y + 30), IM_COL32(22, 25, 37, 255));
		drawList->AddLine(ImVec2(p.x + 50, p.y + 20), ImVec2(p.x + 75, p.y + 30), IM_COL32(22, 25, 37, 255));
	}
	else {
		drawList->AddRectFilled(ImVec2(p.x + 20, p.y), ImVec2(p.x + 60, p.y + 70), IM_COL32(142, 168, 195, 255));
		drawList->AddLine(ImVec2(p.x + 25, p.y + 10), ImVec2(p.x + 55, p.y + 10), IM_COL32(22, 25, 37, 255));
		drawList->AddLine(ImVec2(p.x + 25, p.y + 20), ImVec2(p.x + 55, p.y + 20), IM_COL32(22, 25, 37, 255));
		drawList->AddLine(ImVec2(p.x + 25, p.y + 30), ImVec2(p.x + 55, p.y + 30), IM_COL32(22, 25, 37, 255));
		drawList->AddLine(ImVec2(p.x + 25, p.y + 40), ImVec2(p.x + 55, p.y + 40), IM_COL32(22, 25, 37, 255));
	}

	if (ImGui::InvisibleButton(name.c_str(), ImVec2(100, 70))) {
		EventData data{ EventType::FileSelected };
		data.filePath = fs::path(currentPath + "\\" + name);

		EventManager::getInstance().notifyObservers(data);
	}

	drawList->AddText(ImVec2(p.x, p.y + 75), IM_COL32(255, 255, 255, 255), name.c_str());
}

void FileBrowserWindow::render() {
	int windowWidth, windowHeight;
	glfwGetWindowSize(Window::getInstance().getWindow(), &windowWidth, &windowHeight);

	setupRender();

	ImGui::Begin("File Browser", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	addFolder("...", "Back", true);
	ImGui::SameLine();
	for (const auto& item : getDirectoryIterator(currentPath)) {
		if (item.path().extension().string() == "") {
			addFolder(item.path().filename().string(), "Open Folder");
		}
		else {
			addFile(item.path().filename().string(), item.path().extension().string());
		}
		if (ImGui::GetCursorScreenPos().x < (width - 150)) {
			ImGui::SameLine();
		}
		else {
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
		}
	}

	teardownRender();
}
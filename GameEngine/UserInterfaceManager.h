#ifndef USERINTERFACEMANAGER_H
#define USERINTERFACEMANAGER_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Window.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include "EventManager.h"
#include "UIWindow.h"
#include "InspectorWindow.h"
#include "HierarchyWindow.h"
#include "FileBrowserWindow.h"
#include "SceneWindow.h"

class UserInterfaceManager {
public:
	std::vector<UIWindow*> windows;
	InspectorWindow inspectorWindow;
	HierarchyWindow hierarchyWindow;
	FileBrowserWindow fileBrowserWindow;
	SceneWindow sceneWindow;

	static UserInterfaceManager& getInstance() {
		static UserInterfaceManager instance;
		return instance;
	}

	void initialize(GLFWwindow* window) {
		try {
			if (!window){
				throw std::runtime_error("Failed to initialize ImGUI: GLFW Window has not been initialized");
			}

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init();
		}
		catch (const std::exception& e) {
			std::cerr << "Exception caught: " << e.what() << std::endl;
		}
	}

	void newFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void render() {
		for (auto& window : windows) {
			window->render();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void addWindow(UIWindow* window) {
		windows.push_back(window);
	}

	void editorLayout() {
		addWindow(&inspectorWindow);
		addWindow(&hierarchyWindow);
		addWindow(&fileBrowserWindow);
		addWindow(&sceneWindow);

		EventManager::getInstance().addObserver(&sceneWindow);
	}

private:
	UserInterfaceManager() {}
	UserInterfaceManager(const UserInterfaceManager&) = delete;
	UserInterfaceManager& operator=(const UserInterfaceManager&) = delete;
	~UserInterfaceManager() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
};

#endif

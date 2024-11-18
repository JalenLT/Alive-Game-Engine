#pragma once
#ifndef USERINTERFACEMANAGER_H
#define USERINTERFACEMANAGER_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
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

	static UserInterfaceManager& getInstance();

	void initialize(GLFWwindow* window);

	void newFrame();

	void render();

	void addWindow(UIWindow* window);

	void editorLayout();

	void playbackLayout();

private:
	UserInterfaceManager();
	UserInterfaceManager(const UserInterfaceManager&) = delete;
	UserInterfaceManager& operator=(const UserInterfaceManager&) = delete;
	~UserInterfaceManager();
};

#endif

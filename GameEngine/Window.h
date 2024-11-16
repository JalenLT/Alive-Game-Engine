#pragma once
#ifndef WINDOW_CPP
#define WINDOW_CPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "EventManager.h"

class Window {
public:
	double mouseX = 0.0, mouseY = 0.0;

	static Window& getInstance();

	void initialize(int width, int height, const char* title);

	void fullscreen();

	void close();

	void minimize();

	void restore();

	const GLFWvidmode* getMode();

	GLFWmonitor* getMonitor();

	GLFWwindow* getWindow();

	int getWindowWidth();

	int getWindowHeight();

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	void updateMousePosition();

private:
	GLFWwindow* window = nullptr;
	GLFWmonitor* monitor = nullptr;
	const GLFWvidmode* mode = nullptr;

	Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	~Window();
};

#endif
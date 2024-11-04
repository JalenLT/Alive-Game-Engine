#pragma once
#ifndef WINDOW_CPP
#define WINDOW_CPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
	static Window& getInstance();

	void initialize(int width, int height, const char* title);

	void fullscreen();

	void close();

	void minimize();

	void restore();

	const GLFWvidmode* getMode();

	GLFWmonitor* getMonitor();

	GLFWwindow* getWindow();

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
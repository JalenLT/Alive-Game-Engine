#pragma once
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Window.h"
#include <functional>

class InputManager {
public:
	static InputManager& getInstance() {
		static InputManager instance;
		return instance;
	}

	void registerKeyCallback(GLFWwindow* window, std::function<void(int, int, int, int)> callback) {
		glfwSetKeyCallback(window, [&callback](GLFWwindow* window, int key, int scancode, int action, int mods) {
			callback(key, scancode, action, mods);
		});
	}

private:
	InputManager() {}
	~InputManager() {}

	InputManager(const InputManager& other) = delete;
	InputManager& operator=(const InputManager& other) = delete;
};

#endif

#include "Window.h"

Window& Window::getInstance() {
	static Window instance;
	return instance;
}

void Window::initialize(int width, int height, const char* title) {
	if (!window) {
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

		monitor = glfwGetPrimaryMonitor();

		if (width == 0 && height == 0) {
			window = glfwCreateWindow(getMode()->width, getMode()->height, title, nullptr, nullptr);
		}
		else {
			window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		}

		glfwMakeContextCurrent(window);

		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		glEnable(GL_DEPTH_TEST);
	}
}

void Window::fullscreen() {
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	if (window) {
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else {
		window = glfwCreateWindow(mode->width, mode->height, "", monitor, nullptr);
		glfwMakeContextCurrent(window);
	}
}

void Window::close() {
	if (window) {
		glfwDestroyWindow(window);
	}
}

void Window::minimize() {
	if (window && !glfwGetWindowAttrib(window, GLFW_ICONIFIED)) {
		glfwIconifyWindow(window);
	}
}

void Window::restore() {
	if (window) {
		glfwRestoreWindow(window);
	}
}

const GLFWvidmode* Window::getMode() {
	if (mode == nullptr) {
		mode = glfwGetVideoMode(monitor);
	}
	return mode;
}

GLFWmonitor* Window::getMonitor() {
	return monitor;
}

GLFWwindow* Window::getWindow() {
	return window;
}

int Window::getWindowWidth() {
	int width, height;
	glfwGetWindowSize(Window::getInstance().getWindow(), &width, &height);
	return width;
}

int Window::getWindowHeight() {
	int width, height;
	glfwGetWindowSize(Window::getInstance().getWindow(), &width, &height);
	return height;
}

Window::Window() {}

Window::~Window() {
	if (window) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}
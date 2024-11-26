#include "Window.h"
#include "Renderer.h"

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
		glfwSwapInterval(1);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

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

		glfwSetKeyCallback(Window::getInstance().getWindow(), Window::getInstance().keyCallback);
		glfwSetMouseButtonCallback(Window::getInstance().getWindow(), Window::getInstance().mouseButtonCallback);
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

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		std::cout << "Key pressed: " << key << std::endl;
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, true); // Close the window if ESC is pressed
		}
	}
	else if (action == GLFW_RELEASE) {
		std::cout << "Key released: " << key << std::endl;
	}
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		EventData data = { EventType::MouseLeftClicked };
		data.mousePosition = { Window::getInstance().mouseX, Window::getInstance().mouseY };

		EventManager::getInstance().notifyObservers(data);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {}
	else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		EventData data = { EventType::MouseRightClicked };
		data.mousePosition = { Window::getInstance().mouseX, Window::getInstance().mouseY };

		EventManager::getInstance().notifyObservers(data);
	}
}

void Window::updateMousePosition() {
	glfwGetCursorPos(window, &Window::getInstance().mouseX, &Window::getInstance().mouseY);
}

Window::Window() {}

Window::~Window() {
	if (window) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}
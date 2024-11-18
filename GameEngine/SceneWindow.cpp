#include "SceneWindow.h"
#include "Window.h"
#include "imgui.h"
#include "Renderer.h"

SceneWindow::SceneWindow(float x, float y, float width, float height) : UIWindow(x, y, width, height) {}

void SceneWindow::update(const EventData& data) {
	if (data.type == EventType::MouseClicked) {
		std::cout << data.mousePosition.value()[0] << ", " << data.mousePosition.value()[1] << std::endl;

		Raycast raycast{ Window::getInstance().mouseX, Window::getInstance().mouseY, Window::getInstance().getWindowWidth(), Window::getInstance().getWindowHeight(), Renderer::getInstance().getProjection(), Renderer::getInstance().getView() };

		glm::vec3 min = glm::vec3(-0.5f, -0.5f, -0.5f);
		glm::vec3 max = glm::vec3(0.5f, 0.5f, 0.5f);

		if (raycast.rayIntersectAABB(min, max)) {
			std::cout << "Ray has hit successfully" << std::endl;
		}
	}
}

void SceneWindow::render() {
	int windowWidth, windowHeight;
	glfwGetWindowSize(Window::getInstance().getWindow(), &windowWidth, &windowHeight);

	setupRender();

	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

	teardownRender();

	glViewport(x, Window::getInstance().getWindowHeight() - y - height, width, height);
}
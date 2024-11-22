#include "SceneWindow.h"
#include "Window.h"
#include "imgui.h"
#include "Renderer.h"
#include "SceneManager.h"

SceneWindow::SceneWindow(float x, float y, float width, float height) : UIWindow(x, y, width, height) {}

void SceneWindow::update(const EventData& data) {
	if (data.type == EventType::MouseClicked) {
		std::cout << data.mousePosition.value()[0] << ", " << data.mousePosition.value()[1] << std::endl;
		Raycast raycast{ Window::getInstance().mouseX - x, Window::getInstance().mouseY - y, static_cast<int>(width), static_cast<int>(height), Renderer::getInstance().getProjection(), Renderer::getInstance().getView()};

		for (const auto& gameObject : SceneManager::getInstance().currentScene->gameObjects) {
			if (raycast.rayIntersectAABB(gameObject->boundingBox.min, gameObject->boundingBox.max)) {
				std::cout << "Ray has hit successfully" << std::endl;
				EventData data = { EventType::GameObjectSelected };
				data.gameObjectIndex = gameObject->id;
				EventManager::getInstance().notifyObservers(data);
			}
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
#include "SceneWindow.h"
#include "Window.h"
#include "imgui.h"
#include "Renderer.h"
#include "SceneManager.h"

SceneWindow::SceneWindow(float x, float y, float width, float height) : UIWindow(x, y, width, height) {}

void SceneWindow::update(const EventData& data) {
	if (data.type == EventType::MouseLeftClicked) {
		std::cout << data.mousePosition.value()[0] << ", " << data.mousePosition.value()[1] << std::endl;
		Raycast raycast{ Window::getInstance().mouseX - x, Window::getInstance().mouseY - y, static_cast<int>(width), static_cast<int>(height), Renderer::getInstance().getProjection(), Renderer::getInstance().getView()};

		for (const auto& gameObject : SceneManager::getInstance().currentScene->gameObjects) {
			glm::vec3 newMin = gameObject->boundingBox.min + gameObject->transform.position;
			glm::vec3 newMax = gameObject->boundingBox.max + gameObject->transform.position;
			if (raycast.rayIntersectAABB(newMin, newMax)) {
				std::cout << "Ray has hit successfully" << std::endl;
				EventData data = { EventType::GameObjectSelected };
				data.gameObjectIndex = gameObject->id;
				EventManager::getInstance().notifyObservers(data);
			}
		}
	}
	else if (data.type == EventType::GameObjectSelected) {
		GameObject& gameObject = *SceneManager::getInstance().currentScene->gameObjects[data.gameObjectIndex.value()].get();
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
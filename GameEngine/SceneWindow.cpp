#include "SceneWindow.h"
#include "Window.h"
#include "imgui.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "project_constants.h"

SceneWindow::SceneWindow(float x, float y, float width, float height) : UIWindow(x, y, width, height) {}

void SceneWindow::update(const EventData& data) {
	if (data.type == EventType::MouseLeftClicked) {
		Raycast raycast{ Window::getInstance().mouseX - x, Window::getInstance().mouseY - y, static_cast<int>(width), static_cast<int>(height), Renderer::getInstance().getProjection(), Renderer::getInstance().getView()};
		auto& scene = SceneManager::getInstance().currentScene;
		auto& gameObjects = scene->gameObjects;

		for (size_t i = 0; i < gameObjects.size(); i++) {
			auto& gameObject = gameObjects[i];
			std::cout << "GameObject ID: " << gameObjects.size() << std::endl;

			if (gameObject->containsTag(gameObject->tags, Tags::CanSelect)) {
				glm::vec3 newMin = gameObject->boundingBox.min + gameObject->transform.position;
				glm::vec3 newMax = gameObject->boundingBox.max + gameObject->transform.position;

				if (raycast.rayIntersectAABB(newMin, newMax)) {
					EventData data = { EventType::GameObjectSelected };
					data.gameObjectIndex = gameObject->id;
					EventManager::getInstance().notifyObservers(data);
				}
			}
		}

		/*for (const auto& gameObject : SceneManager::getInstance().currentScene->gameObjects) {
			std::cout << "GameObject ID: " << SceneManager::getInstance().currentScene->gameObjects.size() << std::endl;
			if (gameObject->containsTag(gameObject->tags, Tags::CanSelect)) {
				glm::vec3 newMin = gameObject->boundingBox.min + gameObject->transform.position;
				glm::vec3 newMax = gameObject->boundingBox.max + gameObject->transform.position;
				if (raycast.rayIntersectAABB(newMin, newMax)) {
					EventData data = { EventType::GameObjectSelected };
					data.gameObjectIndex = gameObject->id;
					EventManager::getInstance().notifyObservers(data);
				}
			}
		}*/
	}
	else if (data.type == EventType::GameObjectSelected) {
		GameObject& gameObject = *SceneManager::getInstance().currentScene->gameObjects[data.gameObjectIndex.value()].get();
		bool canContinue = true;
		if (gameObject.children.size() != 0) {
			for (auto& child : gameObject.children) {
				if (auto childPtr = child.lock()) {
					if (childPtr->name == "__arrows") {
						canContinue = false;
					}
				}
			}
		}

		if (canContinue) {
			SceneManager::getInstance().currentScene->addGameObject(ROAMING_PATH + "\\" + PROJECT_NAME + "\\GameObjects\\arrows.glb", "__arrows");
			std::shared_ptr<GameObject> arrow = SceneManager::getInstance().currentScene->gameObjects.back();
			arrow->transform.position = gameObject.transform.position;
			arrow->boundingBox.computeBoundingBox(arrow->transform, arrow->mesh);
			arrow->tags.clear();
			arrow->tags = { Tags::CanRender, Tags::RenderLine };
			arrow->parent = std::weak_ptr<GameObject>(
				SceneManager::getInstance().currentScene->gameObjects[data.gameObjectIndex.value()]
			);
			gameObject.children.push_back(arrow);
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
#include "HierarchyWindow.h"
#include "Window.h"
#include "imgui.h"
#include <string>

HierarchyWindow::HierarchyWindow(float x, float y, float width, float height) : UIWindow(x, y, width, height) {}

void HierarchyWindow::update(const EventData& data) {
    if (data.type == EventType::RefreshSceneHierarchy) {
        scene = SceneManager::getInstance().currentScene;
    }
}

void HierarchyWindow::render() {
    int windowWidth, windowHeight;
    glfwGetWindowSize(Window::getInstance().getWindow(), &windowWidth, &windowHeight);

    setupRender();

    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (scene != nullptr) {
        for (auto& light : scene->lights) {
            bool isOpen = ImGui::TreeNode((light->type + std::to_string(light->id)).c_str());

            if (ImGui::IsItemClicked()) { // Check if clicked and not just toggled
                EventData data = { EventType::LightSelected };
                data.lightIndex = light->id;
                EventManager::getInstance().notifyObservers(data);
            }

            if (isOpen) {
                ImGui::TreePop(); // Close the TreeNode here
            }
        }
        for (auto& gameObject : scene->gameObjects) {
            bool isOpen = ImGui::TreeNode(gameObject->name.c_str());

            if (ImGui::IsItemClicked()) { // Check if clicked and not just toggled
                EventData data = { EventType::GameObjectSelected };
                data.gameObjectIndex = gameObject.get()->id;
                EventManager::getInstance().notifyObservers(data);
            }

            if (isOpen) {
                ImGui::TreePop(); // Close the TreeNode here
            }
        }
    }

    teardownRender();
}
#include "HierarchyWindow.h"
#include "Window.h"
#include "imgui.h"
#include <string>

HierarchyWindow::HierarchyWindow(float x, float y, float width, float height) : UIWindow(x, y, width, height) {}

void HierarchyWindow::update(const EventData& data) {
    if (data.type == EventType::RefreshSceneHierarchy) {
        scene = SceneManager::getInstance().currentScene;
    }
    else if (data.type == EventType::MouseLeftClicked) {
        if (!ImGui::IsAnyItemHovered()) {
			callback = nullptr;
        }
    }
    else if (data.type == EventType::MouseRightClicked) {
        callback = [this, data]() mutable {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.086f, 0.098f, 0.145f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.056f, 0.038f, 0.015f, 1.0f));

            ImGui::SetNextWindowPos(ImVec2(data.mousePosition.value()[0], data.mousePosition.value()[1]), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(Window::getInstance().getWindowWidth() * 0.1, Window::getInstance().getWindowHeight() * 0.33));

            ImGui::Begin("Hierarchy Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse || ImGuiWindowFlags_NoTitleBar);

			if (ImGui::Button("Add GameObject")) {
                std::cout << "clicked add gameobject" << std::endl;
                EventData data = { EventType::AddGameObject };
                EventManager::getInstance().notifyObservers(data);
                this->callback = nullptr;
			}
			if (ImGui::Button("Add Light")) {
				EventData data = { EventType::AddLight };
				EventManager::getInstance().notifyObservers(data);
			}


            ImGui::End();
            ImGui::PopStyleColor(2);
        };
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

    if (callback != nullptr) {
        callback();
    }

    teardownRender();
}
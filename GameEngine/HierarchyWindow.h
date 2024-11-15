#pragma once
#ifndef HIERARCHYWINDOW_H
#define HIERARCHYWINDOW_H

#include "UIWindow.h"
#include "Observer.h"
#include "SceneManager.h"
#include "EventManager.h"

class HierarchyWindow : public UIWindow, public Observer {
public:
	std::shared_ptr<Scene> scene = nullptr;

	HierarchyWindow(float x = 0.0f, float y = 0.0f, float width = 300.0f, float height = 400.0f) : UIWindow(x, y, width, height) {}

	void update(const EventData& data) override {
		if (data.type == EventType::RefreshSceneHierarchy) {
			scene = SceneManager::getInstance().currentScene;
		}
	}
	
	void render() override {
		int windowWidth, windowHeight;
		glfwGetWindowSize(Window::getInstance().getWindow(), &windowWidth, &windowHeight);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

		ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(width, height));
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
		
		ImGui::End();

		ImGui::PopStyleColor(2);
	}
};

#endif
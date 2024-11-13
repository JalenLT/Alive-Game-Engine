#pragma once
#ifndef INSPECTORWINDOW_H
#define INSPECTORWINDOW_H

#define GLM_ENABLE_EXPERIMENTAL
#include "UIWindow.h"
#include "Observer.h"
#include "SceneManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class InspectorWindow : public UIWindow, public Observer {
public:
	InspectorWindow(float x = 0.0f, float y = 0.0f, float width = 300.0f, float height = 400.0f) : UIWindow(x, y, width, height) {};

	void update(const EventData& data) override {
		if (data.type == EventType::FileSelected) {
			callback = [&]() {
				ImGui::Text("GameObject");
				ImGui::Text("Path: ");
				ImGui::SameLine();
				if (data.filePath) {
					std::string pathStr = data.filePath->string();
					ImGui::Text(pathStr.c_str());
				}
				else {
					ImGui::Text("No file path available.");
				}

			};
		}
		else if (data.type == EventType::GameObjectSelected) {
			callback = [data]() mutable {
				float rotation[3] = { 0.0f, 0.0f, 0.0f };

				GameObject* gameObject = SceneManager::getInstance().currentScene->gameObjects[data.gameObjectIndex.value()].get();

				ImGui::Text(("GameObject: " + gameObject->name).c_str());

				ImGui::Text("Position: ");
				ImGui::Columns(3);
				ImGui::InputFloat("lX", &gameObject->modelMatrix[3][0], 0.1f);
				ImGui::NextColumn();
				ImGui::InputFloat("lY", &gameObject->modelMatrix[3][1], 0.1f);
				ImGui::NextColumn();
				ImGui::InputFloat("lZ", &gameObject->modelMatrix[3][2], 0.1f);
				ImGui::NextColumn();
				ImGui::Columns(1);

				ImGui::Text("Rotation: ");
				ImGui::Columns(3);
				ImGui::InputFloat("rX", &rotation[0], 0.1f);
				ImGui::NextColumn();
				ImGui::InputFloat("rY", &rotation[1], 0.1f);
				ImGui::NextColumn();
				ImGui::InputFloat("rZ", &rotation[2], 0.1f);
				ImGui::NextColumn();
				ImGui::Columns(1);

				// Create a quaternion representing a rotation (e.g., 45 degrees around the y-axis)
				glm::quat rotationQuat = glm::angleAxis(glm::radians(45.0f), glm::vec3(rotation[0], rotation[1], rotation[2]));

				// Convert the quaternion to a rotation matrix
				glm::mat4 rotationMatrix = glm::toMat4(rotationQuat);

				gameObject->modelMatrix = rotationMatrix * gameObject->modelMatrix;
			};
		}
	}
	
	void render() override {
		int windowWidth, windowHeight;
		glfwGetWindowSize(Window::getInstance().getWindow(), &windowWidth, &windowHeight);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

		ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(width, height));
		ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		if (callback != nullptr) {
			callback();
		}
		else {
			ImGui::Text("...");
		}

		ImGui::End();

		ImGui::PopStyleColor(2);
	}

private:
	std::function<void()> callback = nullptr;
};

#endif
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

	void transform(Transform& transform) {
		glm::vec3 currentRotation = transform.getRotation();
		glm::vec3 newRotation = currentRotation;

		ImGui::Text("Position: ");
		ImGui::Columns(3);
		ImGui::InputFloat("lX", &transform.position[0], 0.1f);
		ImGui::NextColumn();
		ImGui::InputFloat("lY", &transform.position[1], 0.1f);
		ImGui::NextColumn();
		ImGui::InputFloat("lZ", &transform.position[2], 0.1f);
		ImGui::NextColumn();
		ImGui::Columns(1);

		ImGui::Text("Rotation: ");
		ImGui::Columns(3);
		if (ImGui::InputFloat("rX", &newRotation.x, 0.02f)) {
			float diffX = newRotation.x - currentRotation.x;
			transform.rotateAroundAxisAngle(glm::vec3(1.0f, 0.0f, 0.0f), 360.0f * diffX);
		}
		ImGui::NextColumn();
		if (ImGui::InputFloat("rY", &newRotation.y, 0.02f)) {
			float diffY = newRotation.y - currentRotation.y;
			transform.rotateAroundAxisAngle(glm::vec3(0.0f, 1.0f, 0.0f), 360.0f * diffY);
		}
		ImGui::NextColumn();
		if (ImGui::InputFloat("rZ", &newRotation.z, 0.02f)) {
			float diffZ = newRotation.z - currentRotation.z;
			transform.rotateAroundAxisAngle(glm::vec3(0.0f, 0.0f, 1.0f), 360.0f * diffZ);
		}
		ImGui::NextColumn();
		ImGui::Columns(1);
	}

	void material(Material& material) {
		ImGui::Text("Ambient: ");
		ImGui::Columns(3);
		ImGui::InputFloat("aR", &material.ambient.x, 0.1f);
		ImGui::NextColumn();
		ImGui::InputFloat("aG", &material.ambient.y, 0.1f);
		ImGui::NextColumn();
		ImGui::InputFloat("aB", &material.ambient.z, 0.1f);
		ImGui::NextColumn();
		ImGui::Columns(1);

		ImGui::Text("Diffuse: ");
		ImGui::Columns(3);
		ImGui::InputFloat("dR", &material.diffuse.x, 0.1f);
		ImGui::NextColumn();
		ImGui::InputFloat("dG", &material.diffuse.y, 0.1f);
		ImGui::NextColumn();
		ImGui::InputFloat("dB", &material.diffuse.z, 0.1f);
		ImGui::NextColumn();
		ImGui::Columns(1);

		ImGui::Text("Specular: ");
		ImGui::Columns(3);
		ImGui::InputFloat("sR", &material.specular.x, 0.1f);
		ImGui::NextColumn();
		ImGui::InputFloat("sG", &material.specular.y, 0.1f);
		ImGui::NextColumn();
		ImGui::InputFloat("sB", &material.specular.z, 0.1f);
		ImGui::NextColumn();
		ImGui::Columns(1);
	}

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
			callback = [this, data]() mutable {
				GameObject* gameObject = SceneManager::getInstance().currentScene->gameObjects[data.gameObjectIndex.value()].get();
				ImGui::Text(gameObject->name.c_str());
				this->transform(gameObject->transform);
				ImGui::NewLine();
				ImGui::NewLine();
				this->material(gameObject->material);
			};
		}
		else if (data.type == EventType::LightSelected) {
			callback = [this, data]() mutable {
				Light* light = SceneManager::getInstance().currentScene->lights[data.lightIndex.value()].get();
				ImGui::Text((light->type + std::to_string(light->id)).c_str());
				this->transform(light->transform);
				ImGui::NewLine();
				ImGui::NewLine();
				this->material(light->material);
			};
		}
	}
	
	void render() override {
		int windowWidth, windowHeight;
		glfwGetWindowSize(Window::getInstance().getWindow(), &windowWidth, &windowHeight);

		setupRender();

		ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		if (callback != nullptr) {
			callback();
		}
		else {
			ImGui::Text("...");
		}

		teardownRender();
	}

private:
	std::function<void()> callback = nullptr;
};

#endif
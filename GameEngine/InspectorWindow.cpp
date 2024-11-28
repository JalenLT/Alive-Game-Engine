#include "InspectorWindow.h"
#include "Window.h"
#include "imgui.h"
#include <iostream>
#include <string>

InspectorWindow::InspectorWindow(float x, float y, float width, float height) : UIWindow(x, y, width, height) {}

void InspectorWindow::transform(Transform& transform) {
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

void InspectorWindow::material(Material& material, const std::string& uniqueID) {
	ImGui::Text("Ambient: ");
	ImGui::Columns(3);
	ImGui::InputFloat(("R##a" + uniqueID).c_str(), &material.ambient.x, 0.1f);
	ImGui::NextColumn();
	ImGui::InputFloat(("G##a" + uniqueID).c_str(), &material.ambient.y, 0.1f);
	ImGui::NextColumn();
	ImGui::InputFloat(("B##a" + uniqueID).c_str(), &material.ambient.z, 0.1f);
	ImGui::NextColumn();
	ImGui::Columns(1);

	ImGui::Text("Diffuse: ");
	ImGui::Columns(3);
	ImGui::InputFloat(("R##d" + uniqueID).c_str(), &material.diffuse.x, 0.1f);
	ImGui::NextColumn();
	ImGui::InputFloat(("G##d" + uniqueID).c_str(), &material.diffuse.y, 0.1f);
	ImGui::NextColumn();
	ImGui::InputFloat(("B##d" + uniqueID).c_str(), &material.diffuse.z, 0.1f);
	ImGui::NextColumn();
	ImGui::Columns(1);

	ImGui::Text("Specular: ");
	ImGui::Columns(3);
	ImGui::InputFloat(("R##s" + uniqueID).c_str(), &material.specular.x, 0.1f);
	ImGui::NextColumn();
	ImGui::InputFloat(("G##s" + uniqueID).c_str(), &material.specular.y, 0.1f);
	ImGui::NextColumn();
	ImGui::InputFloat(("B##s" + uniqueID).c_str(), &material.specular.z, 0.1f);
	ImGui::NextColumn();
	ImGui::Columns(1);
}

void InspectorWindow::update(const EventData& data) {
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
			std::string gameObjectName = gameObject->name;
			if (ImGui::InputText("Name", &gameObjectName[0], sizeof(gameObjectName))) {
				gameObject->name = gameObjectName;
			}
			this->transform(gameObject->transform);
			ImGui::NewLine();
			ImGui::NewLine();
			for(int i = 0; i < gameObject->mesh.subMeshes.size(); i++) {
				this->material(gameObject->mesh.subMeshes[i].material, std::to_string(i));
			}
		};
	}
	else if (data.type == EventType::LightSelected) {
		callback = [this, data]() mutable {
			Light* light = SceneManager::getInstance().currentScene->lights[data.lightIndex.value()].get();
			ImGui::Text((light->type + std::to_string(light->id)).c_str());
			this->transform(light->transform);
			ImGui::NewLine();
			ImGui::NewLine();
			this->material(light->material, "1");
		};
	}
}

void InspectorWindow::render() {
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
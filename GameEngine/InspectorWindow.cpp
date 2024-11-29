#include "InspectorWindow.h"
#include "Window.h"
#include "imgui.h"
#include <iostream>
#include <string>

InspectorWindow::InspectorWindow(float x, float y, float width, float height) : UIWindow(x, y, width, height) {}

void InspectorWindow::transform(Transform& transform) {
	glm::vec3 currentRotation = transform.getRotation();
	glm::vec3 newRotation = currentRotation;

	float position[3] = { transform.position[0], transform.position[1], transform.position[2] };
	float rotation[3] = { currentRotation.x, currentRotation.y, currentRotation.z };
	float scale[3] = { transform.scale[0], transform.scale[1], transform.scale[2] };

	if (ImGui::InputFloat3("Position", position)) {
		if (ImGui::IsItemDeactivatedAfterEdit()) {
			EventData data = { EventType::UpdateTransform };
			if(transform.parentType == "GameObject") data.gameObjectIndex = transform.parentId;
			else if(transform.parentType == "Light") data.lightIndex = transform.parentId;
			data.position = glm::vec3(position[0], position[1], position[2]);
			EventManager::getInstance().notifyObservers(data);
		}
	}
	if (ImGui::InputFloat3("Rotation", rotation)) {
		if (ImGui::IsItemDeactivatedAfterEdit()) {
			EventData data = { EventType::UpdateTransform };
			if (transform.parentType == "GameObject") data.gameObjectIndex = transform.parentId;
			else if (transform.parentType == "Light") data.lightIndex = transform.parentId;
			data.rotation = glm::vec3(rotation[0], rotation[1], rotation[2]);
			EventManager::getInstance().notifyObservers(data);
		}
	}
	if (ImGui::InputFloat3("Scale", scale)) {
		if (ImGui::IsItemDeactivatedAfterEdit()) {
			EventData data = { EventType::UpdateTransform };
			if (transform.parentType == "GameObject") data.gameObjectIndex = transform.parentId;
			else if (transform.parentType == "Light") data.lightIndex = transform.parentId;
			data.scale = glm::vec3(scale[0], scale[1], scale[2]);
			EventManager::getInstance().notifyObservers(data);
		}
	}
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

	/*float specularColor[3] = { material.specular.x, material.specular.y, material.specular.z };
	if (ImGui::ColorPicker3("Specular Color", specularColor)) {
		material.specular = glm::vec3(specularColor[0], specularColor[1], specularColor[2]);
	}*/
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
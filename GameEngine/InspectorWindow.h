#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#ifndef INSPECTORWINDOW_H
#define INSPECTORWINDOW_H

#include "UIWindow.h"
#include "Observer.h"
#include "SceneManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/component_wise.hpp>
#include <functional>
#include <string>


class InspectorWindow : public UIWindow, public Observer {
public:
	InspectorWindow(float x = 0.0f, float y = 0.0f, float width = 300.0f, float height = 400.0f);

	void transform(Transform& transform);

	void material(Material& material, const std::string& uniqueID);

	void update(const EventData& data) override;
	
	void render() override;

private:
	std::function<void()> callback = nullptr;
};

#endif
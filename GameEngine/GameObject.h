#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Observer.h"
#include "Material.h"

class GameObject : public Observer {
public:
	unsigned int VAO, VBO, EBO;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	Material material;

	void update(const std::string& event) override {}
};

#endif
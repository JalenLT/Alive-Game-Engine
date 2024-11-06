#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Material {
	glm::vec3 ambient = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	float shininess = 10.0f;
};

#endif
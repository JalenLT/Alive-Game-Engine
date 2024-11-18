#pragma once
#ifndef RAYCAST_H
#define RAYCAST_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Raycast {
public:
	glm::vec3 rayDirection;
	glm::vec3 rayOrigin;

	Raycast(double mouseX, double mouseY, int screenWidth, int screenHeight, glm::mat4 projection, glm::mat4 view);
	~Raycast();

	bool rayIntersectAABB(glm::vec3& min, glm::vec3& max);

private:
	double mouseX, mouseY;
	double screenWidth, screenHeight;
};

#endif

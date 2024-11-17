#pragma once
#ifndef RAYCAST_H
#define RAYCAST_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Raycast {
public:
	glm::vec3 rayDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rayOrigin = glm::vec3(0.0f, 0.0f, 0.0f);

	Raycast(double mouseX, double mouseY, int screenWidth, int screenHeight, glm::mat4 projection, glm::mat4 view) : mouseX(mouseX), mouseY(mouseY), screenWidth(screenWidth), screenHeight(screenHeight) {
		float x = (2.0f * mouseX) / screenWidth - 1.0f;
		float y = 1.0f - (2.0f * mouseY) / screenHeight;
		float z = 1.0f;

		glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);
		glm::vec4 rayEye = glm::inverse(projection) * rayClip;
		rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
		glm::vec3 rayWorld = glm::vec3(glm::inverse(view) * rayEye);
		rayWorld = glm::normalize(rayWorld);
		
		rayDirection = rayWorld;
		glm::mat4 inverseView = glm::inverse(view);
		rayOrigin = glm::vec3(inverseView[3][0], inverseView[3][1], inverseView[3][2]);
	}
	~Raycast() {}

	bool rayIntersectAABB(glm::vec3& min, glm::vec3& max) {
		float tXMin, tYMin, tZMin, tXMax, tYMax, tZMax;

		if (rayDirection.x != 0.0f) {
			tXMin = (min.x - rayOrigin.x) / rayDirection.x;
			tXMax = (max.x - rayOrigin.x) / rayDirection.x;

			if (tXMin > tXMax) {
				std::swap(tXMin, tXMax);
			}
		}
		else {
			tXMin = (rayOrigin.x >= min.x && rayOrigin.x <= max.x) ? -INFINITY : INFINITY;
			tXMax = -tXMin;
		}

		if (rayDirection.y != 0.0f) {
			tYMin = (min.y - rayOrigin.y) / rayDirection.y;
			tYMax = (max.y - rayOrigin.y) / rayDirection.y;

			if (tYMin > tYMax) {
				std::swap(tYMin, tYMax);
			}
		}
		else {
			tYMin = (rayOrigin.y >= min.y && rayOrigin.y <= max.y) ? -INFINITY : INFINITY;
			tYMax = -tYMin;
		}

		if (rayDirection.z != 0.0f) {
			tZMin = (min.z - rayOrigin.z) / rayDirection.z;
			tZMax = (max.z - rayOrigin.z) / rayDirection.z;

			if (tZMin > tZMax) {
				std::swap(tZMin, tZMax);
			}
		}
		else {
			tZMin = (rayOrigin.z >= min.z && rayOrigin.z <= max.z) ? -INFINITY : INFINITY;
			tZMax = -tZMin;
		}

		float tMin = std::max(tXMin, std::max(tYMin, tZMin));
		float tMax = std::min(tXMax, std::min(tYMax, tZMax));

		return tMin <= tMax && tMax >= 0;
	}

private:
	double mouseX, mouseY;
	double screenWidth, screenHeight;
};

#endif

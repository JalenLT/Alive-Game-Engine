#pragma once
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glad/glad.h>

class Mesh {
public:
	Mesh();
	~Mesh() = default;

	unsigned int VAO, VBO, EBO;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	void initializeBuffers(std::vector<float> vertices, const std::vector<unsigned int>& indices = {}, bool containsNormals = true);
};

#endif
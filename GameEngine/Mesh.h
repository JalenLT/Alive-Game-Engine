#pragma once
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glad/glad.h>
#include "glm_serialization.h"
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>

class Mesh {
public:
	Mesh();
	~Mesh() = default;

	unsigned int VAO, VBO, EBO;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	void initializeBuffers(std::vector<float>& vertices, const std::vector<unsigned int>& indices = {}, bool containsNormals = true);

	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			CEREAL_NVP(vertices),
			CEREAL_NVP(indices)
		);
	}
};

#endif
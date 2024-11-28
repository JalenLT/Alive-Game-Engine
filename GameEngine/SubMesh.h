#pragma once
#ifndef SUBMESH_H
#define SUBMESH_H

#include <vector>
#include <glad/glad.h>
#include "glm_serialization.h"
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>
#include "Material.h"

class SubMesh {
public:
	SubMesh();
	~SubMesh() = default;

	unsigned int VAO, VBO, EBO;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	Material material;

	void initializeBuffers(std::vector<float>& vertices, const std::vector<unsigned int>& indices = {}, bool containsNormals = true);

	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			CEREAL_NVP(vertices),
			CEREAL_NVP(indices),
			CEREAL_NVP(material)
		);
	}
};

#endif

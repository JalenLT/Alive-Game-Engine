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
#include "SubMesh.h"

class Mesh {
public:
	Mesh();
	~Mesh() = default;

	std::vector<SubMesh> subMeshes;

	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			CEREAL_NVP(subMeshes)
		);
	}
};

#endif
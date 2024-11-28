#pragma once
#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "glm/glm.hpp"
#include "Transform.h"
#include <vector>
#include "glm_serialization.h"
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>
#include "Mesh.h"

class BoundingBox {
public:
	glm::vec3 min, max;
	std::vector<float> vertices;
	Mesh mesh;

	BoundingBox();
	~BoundingBox() = default;

	void computeBoundingBox(Transform& transform, const Mesh& mesh);

	void computeBoundingBoxVertices();

	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			CEREAL_NVP(min),
			CEREAL_NVP(max)
		);
	}
};

#endif
#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/archives/json.hpp>
#include "glm_serialization.h"

struct Material {
	glm::vec3 ambient = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	float shininess = 10.0f;

	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			CEREAL_NVP(ambient),
			CEREAL_NVP(diffuse),
			CEREAL_NVP(specular),
			CEREAL_NVP(shininess)
		);
	}
};

#endif
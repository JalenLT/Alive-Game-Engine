#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "Transform.h"
#include "Material.h"
#include "glm_serialization.h"
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>
#include <string>
#include <glm/glm.hpp>

class Light {
public:
	Light();
	~Light() = default;

	int id;
	std::string type;
	Transform transform;
	Material material;
	glm::vec3 attenuation;

	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			CEREAL_NVP(id),
			CEREAL_NVP(type),
			CEREAL_NVP(transform),
			CEREAL_NVP(material),
			CEREAL_NVP(attenuation)
		);
	}
};

#endif
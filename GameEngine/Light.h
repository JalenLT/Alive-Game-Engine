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

class Light {
public:
	Light() {}
	~Light() = default;

	int id{ 0 };
	std::string type{ "Point" };
	Transform transform;
	Material material;
	glm::vec3 attenuation{ 1.0f, 0.09f, 0.032f };

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
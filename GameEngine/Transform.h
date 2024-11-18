#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "glm_serialization.h"
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>

class Transform {
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

    Transform();

    glm::mat4 getMatrix() const;

    glm::vec3 getRotation();

    void rotateAroundAxisAngle(glm::vec3 axis, float angle);

    template <class Archive>
    void serialize(Archive& ar) {
        ar(
            CEREAL_NVP(position),
            CEREAL_NVP(rotation),
            CEREAL_NVP(scale)
        );
    }
};

#endif
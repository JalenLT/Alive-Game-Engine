#pragma once
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
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

    Transform()
        : position(0.0f), rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), scale(1.0f) {}

    glm::mat4 getMatrix() const {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMatrix = glm::toMat4(rotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    glm::vec3 getRotation() {
        return glm::degrees(glm::eulerAngles(rotation));
    }

    void rotateAroundAxisAngle(glm::vec3 axis, float angle) {
        glm::quat quaternion = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
        rotation = rotation * quaternion;
        rotation = glm::normalize(rotation);
    }

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
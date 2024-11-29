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
    int parentId;
    std::string parentType;
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
    
    Transform(int parentId, std::string parentType);

    glm::mat4 getMatrix() const;

    glm::vec3 getRotation();

    void setRotation(const glm::vec3& eulerAngles);

    void rotateAroundAxisAngle(glm::vec3 axis, float angle);

    glm::vec3 applyToPoint(const glm::vec3& point) const;

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
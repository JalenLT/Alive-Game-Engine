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

    /****************
     *** POSITION ***
     ****************/
    glm::vec3 getLocalPosition(Transform& transform);
    glm::vec3 getGlobalPosition();
    glm::vec3 applyToPoint(const glm::vec3& point) const;
    void setLocalPosition(Transform& parentTransform, glm::vec3 localPosition);
    void setGlobalPosition(glm::vec3 globalPosition);
    void translateGlobal(glm::vec3 translation);

    /****************
     *** ROTATION ***
     ****************/
    glm::vec3 getRotation();
    void setRotation(const glm::vec3& eulerAngles);
    void setGlobalRotation(const glm::quat& newRotation);
    void setLocalRotation(Transform& parentTransform, const glm::quat& localRotation);
    glm::quat getGlobalRotation() const;
    glm::quat getLocalRotation(const Transform& parentTransform) const;
    void rotateAroundAxisAngle(glm::vec3 axis, float angle);

    /*************
     *** SCALE ***
     *************/
    void setGlobalScale(const glm::vec3& newScale);
    void setLocalScale(Transform& parentTransform, const glm::vec3& localScale);
    glm::vec3 getGlobalScale() const;
    glm::vec3 getLocalScale(const Transform& parentTransform) const;

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
#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#ifndef GLM_SERIALIZATION_H
#define GLM_SERIALIZATION_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/common.hpp>

namespace cereal {
    //Serialization function for glm::mat4
    template <class Archive>
    void serialize(Archive& ar, glm::mat4& matrix) {
        ar(
            CEREAL_NVP(matrix[0][0]), CEREAL_NVP(matrix[0][1]), CEREAL_NVP(matrix[0][2]), CEREAL_NVP(matrix[0][3]),
            CEREAL_NVP(matrix[1][0]), CEREAL_NVP(matrix[1][1]), CEREAL_NVP(matrix[1][2]), CEREAL_NVP(matrix[1][3]),
            CEREAL_NVP(matrix[2][0]), CEREAL_NVP(matrix[2][1]), CEREAL_NVP(matrix[2][2]), CEREAL_NVP(matrix[2][3]),
            CEREAL_NVP(matrix[3][0]), CEREAL_NVP(matrix[3][1]), CEREAL_NVP(matrix[3][2]), CEREAL_NVP(matrix[3][3])
        );
    }

    // Serialization function for glm::vec2
    template <class Archive>
    void serialize(Archive& ar, glm::vec2& vec) {
        ar(
            cereal::make_nvp("x", vec.x),
            cereal::make_nvp("y", vec.y)
        );
    }

    // Serialization function for glm::vec3
    template <class Archive>
    void serialize(Archive& ar, glm::vec3& vec) {
        ar(
            cereal::make_nvp("x", vec.x),
            cereal::make_nvp("y", vec.y),
            cereal::make_nvp("z", vec.z)
        );
    }

    // Serialization function for glm::vec4
    template <class Archive>
    void serialize(Archive& ar, glm::vec4& vec) {
        ar(
            cereal::make_nvp("x", vec.x),
            cereal::make_nvp("y", vec.y),
            cereal::make_nvp("z", vec.z),
            cereal::make_nvp("w", vec.w)
        );
    }

    // Serialization function for glm::quat
    template <class Archive>
    void serialize(Archive& ar, glm::quat& q) {
        ar(
            CEREAL_NVP(q.w),
            CEREAL_NVP(q.x),
            CEREAL_NVP(q.y),
            CEREAL_NVP(q.z)
        );
    }
}

#endif
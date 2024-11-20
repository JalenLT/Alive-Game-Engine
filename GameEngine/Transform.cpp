#include "Transform.h"

Transform::Transform()
    : position(0.0f), rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), scale(1.0f) {}

glm::mat4 Transform::getMatrix() const {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationMatrix = glm::toMat4(rotation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    return translationMatrix * rotationMatrix * scaleMatrix;
}

glm::vec3 Transform::getRotation() {
    return glm::degrees(glm::eulerAngles(rotation));
}

void Transform::rotateAroundAxisAngle(glm::vec3 axis, float angle) {
    glm::quat quaternion = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
    rotation = rotation * quaternion;
    rotation = glm::normalize(rotation);
}

glm::vec3 Transform::applyToPoint(const glm::vec3& point) const {
    glm::mat4 modelMatrix = getMatrix(); // Assumes you have a function to get the full model matrix
    glm::vec4 transformedPoint = modelMatrix * glm::vec4(point, 1.0f);
    return glm::vec3(transformedPoint);
}
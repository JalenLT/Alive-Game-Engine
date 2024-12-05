#include "Transform.h"

Transform::Transform(int parentId, std::string parentType)
    : parentId(parentId), parentType(parentType), position(0.0f), rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), scale(1.0f) {}

glm::mat4 Transform::getMatrix() const {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationMatrix = glm::toMat4(rotation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    return translationMatrix * rotationMatrix * scaleMatrix;
}

glm::vec3 Transform::getRotation() {
    return glm::degrees(glm::eulerAngles(rotation));
}

void Transform::setRotation(const glm::vec3& eulerAngles) {
    glm::vec3 radians = glm::radians(eulerAngles);
    rotation = glm::quat(radians);
}

glm::vec3 Transform::getLocalPosition(Transform& parentTransform) {
    glm::vec3 relativePosition = position - parentTransform.position;

    glm::quat inverseRotation = glm::inverse(parentTransform.rotation);
    glm::vec3 localPosition = inverseRotation * relativePosition;

    return localPosition;
}

glm::vec3 Transform::getGlobalPosition() {
    return position;
}

void Transform::setGlobalPosition(glm::vec3 globalPosition) {
	position = globalPosition;
}

void Transform::setLocalPosition(Transform& parentTransform, glm::vec3 localPosition) {
    glm::vec3 rotatedPosition = parentTransform.rotation * localPosition;

    position = parentTransform.position + rotatedPosition;
}

void Transform::setGlobalRotation(const glm::quat& newRotation) {
    rotation = newRotation;
}

void Transform::setLocalRotation(Transform& parentTransform, const glm::quat& localRotation) {
    // Combine parent's global rotation with the local rotation
    rotation = parentTransform.rotation * localRotation;
}

void Transform::translateGlobal(glm::vec3 translation) {
    position = position + translation;
}

void Transform::setGlobalScale(const glm::vec3& newScale) {
    scale = newScale;
}

void Transform::setLocalScale(Transform& parentTransform, const glm::vec3& localScale) {
    // Multiply the parent's global scale by the local scale to get the global scale
    scale = parentTransform.scale * localScale;
}

glm::quat Transform::getGlobalRotation() const {
    return rotation;
}

glm::quat Transform::getLocalRotation(const Transform& parentTransform) const {
    glm::quat localRotation = glm::inverse(parentTransform.rotation) * rotation;
    return localRotation;
}

glm::vec3 Transform::getGlobalScale() const {
    return scale;
}

glm::vec3 Transform::getLocalScale(const Transform& parentTransform) const {
    glm::vec3 localScale = scale / parentTransform.scale;
    return localScale;
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
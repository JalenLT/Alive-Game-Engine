#include "BoundingBox.h"

BoundingBox::BoundingBox() : min(glm::vec3(0.0f, 0.0f, 0.0f)), max(glm::vec3(0.0f, 0.0f, 0.0f)) {}

void BoundingBox::computeBoundingBox(Transform& transform, const std::vector<float>& vertices) {
    // Initialize min and max values
    glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 max = glm::vec3(std::numeric_limits<float>::lowest());

    // Process vertices in groups of 3 (x, y, z)
    for (size_t i = 0; i < vertices.size(); i += 3) {
        glm::vec3 localPosition(
            vertices[i],     // x
            vertices[i + 1], // y
            vertices[i + 2]  // z
        );

        // Transform to world space
        glm::vec3 worldPosition = transform.applyToPoint(localPosition);

        // Update min and max values
        min = glm::min(min, worldPosition);
        max = glm::max(max, worldPosition);
    }

    // Store computed bounding box
    this->min = min;
    this->max = max;

    computeBoundingBoxVertices();

    this->mesh.vertices = this->vertices;
    this->mesh.initializeBuffers(this->mesh.vertices, {}, false);
}

void BoundingBox::computeBoundingBoxVertices() {
    if (min != glm::vec3(0.0f, 0.0f, 0.0f) && max != glm::vec3(0.0f, 0.0f, 0.0f)) {
        glm::vec3 corners[8] = {
        glm::vec3(min.x, min.y, min.z), // 0: Bottom-front-left
        glm::vec3(max.x, min.y, min.z), // 1: Bottom-front-right
        glm::vec3(min.x, max.y, min.z), // 2: Top-front-left
        glm::vec3(max.x, max.y, min.z), // 3: Top-front-right
        glm::vec3(min.x, min.y, max.z), // 4: Bottom-back-left
        glm::vec3(max.x, min.y, max.z), // 5: Bottom-back-right
        glm::vec3(min.x, max.y, max.z), // 6: Top-back-left
        glm::vec3(max.x, max.y, max.z)  // 7: Top-back-right
        };

        // Define the 12 edges of the cuboid as pairs of corners
        int edges[24] = {
            // Front face edges
            0, 1, 1, 3, 3, 2, 2, 0,
            // Back face edges
            4, 5, 5, 7, 7, 6, 6, 4,
            // Connecting edges between front and back faces
            0, 4, 1, 5, 2, 6, 3, 7
        };

        // Generate the vertex list
        std::vector<float> vertices;
        for (int i = 0; i < 24; ++i) {
            glm::vec3 corner = corners[edges[i]];
            vertices.push_back(corner.x);
            vertices.push_back(corner.y);
            vertices.push_back(corner.z);
        }

        this->vertices = vertices;
    }
}
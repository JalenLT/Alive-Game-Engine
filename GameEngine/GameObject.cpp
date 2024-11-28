#include "GameObject.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include <glad/glad.h>

GameObject::GameObject() : id(0), name("Cube"), path("tmp") {}
GameObject::~GameObject() = default;

void GameObject::update(const EventData& data) {}

void GameObject::initialize(const int id, const std::string& path, std::shared_ptr<GameObject> parent) {
    this->id = id;
    this->path = path;
    this->parent = parent;

    loadModel(path);

    boundingBox.computeBoundingBox(transform, mesh);
}

void GameObject::initialize(const int id, const Mesh mesh, const Transform& transform, std::shared_ptr<GameObject> parent) {
    this->id = id;
    this->parent = parent;

	this->transform = transform;

    this->mesh = mesh;

    for (auto& subMesh : this->mesh.subMeshes) {
		subMesh.initializeBuffers(subMesh.vertices, subMesh.indices);
	}

    boundingBox.computeBoundingBox(this->transform, mesh);
}

void GameObject::loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Error from ASSIMP: " << importer.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);
}

void GameObject::processNode(aiNode* node, const aiScene* scene) {
    std::cout << "Node NumMeshes: " << node->mNumMeshes << std::endl;

    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        std::cout << "Node Mesh " << i << ": " << scene->mMeshes[node->mMeshes[i]] << std::endl;
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void GameObject::processMesh(aiMesh* mesh, const aiScene* scene) {
    this->mesh.subMeshes.push_back(SubMesh());
    SubMesh& subMesh = this->mesh.subMeshes.back();

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    aiColor4D color;
    if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
		subMesh.material.diffuse = glm::vec3(color.r, color.g, color.b);
    }
    if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, color)) {
        subMesh.material.ambient = glm::vec3(color.r, color.g, color.b);
    }
    if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, color)) {
        subMesh.material.specular = glm::vec3(color.r, color.g, color.b);
    }

    if (subMesh.vertices.size() != 0) {
        subMesh.vertices.clear();
    }
    
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        subMesh.vertices.push_back(mesh->mVertices[i].x);
        subMesh.vertices.push_back(mesh->mVertices[i].y);
        subMesh.vertices.push_back(mesh->mVertices[i].z);

        if (mesh->mNormals) {
            subMesh.vertices.push_back(mesh->mNormals[i].x);
            subMesh.vertices.push_back(mesh->mNormals[i].y);
            subMesh.vertices.push_back(mesh->mNormals[i].z);
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        std::cout << mesh->mMaterialIndex << std::endl;

        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            subMesh.indices.push_back(face.mIndices[j]);
        }
    }

    subMesh.initializeBuffers(subMesh.vertices, subMesh.indices);
}
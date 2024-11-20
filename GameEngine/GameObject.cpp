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

    boundingBox.computeBoundingBox(transform, mesh.vertices);
}

void GameObject::initialize(const int id, const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const Transform& transform, const Material& material, std::shared_ptr<GameObject> parent) {
    this->id = id;
    this->parent = parent;

	this->transform = transform;
	this->material = material;

    this->mesh.vertices = vertices;
	this->mesh.indices = indices;
    this->mesh.initializeBuffers(this->mesh.vertices, this->mesh.indices);

    boundingBox.computeBoundingBox(this->transform, mesh.vertices);
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
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void GameObject::processMesh(aiMesh* mesh, const aiScene* scene) {
    if (this->mesh.vertices.size() != 0) {
        this->mesh.vertices.clear();
    }
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        this->mesh.vertices.push_back(mesh->mVertices[i].x);
        this->mesh.vertices.push_back(mesh->mVertices[i].y);
        this->mesh.vertices.push_back(mesh->mVertices[i].z);

        if (mesh->mNormals) {
            this->mesh.vertices.push_back(mesh->mNormals[i].x);
            this->mesh.vertices.push_back(mesh->mNormals[i].y);
            this->mesh.vertices.push_back(mesh->mNormals[i].z);
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            this->mesh.indices.push_back(face.mIndices[j]);
        }
    }

    this->mesh.initializeBuffers(this->mesh.vertices, this->mesh.indices);
}
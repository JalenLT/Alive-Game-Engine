#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "Observer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "GameObject.h"
#include "Light.h"

class Renderer {
public:
	unsigned int shaderProgram = 0;

	std::vector<std::unique_ptr<GameObject>> gameObjects;

	const char* vertexShaderSource = R"(
		#version 330 core
		layout(location = 0) in vec3 aPos;
		layout(location = 1) in vec3 aNormal;

		out vec3 FragPos;
		out vec3 Normal;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main(){
		    FragPos = vec3(model * vec4(aPos, 1.0));
			Normal = mat3(transpose(inverse(model))) * aNormal;

			gl_Position = projection * view * model * vec4(aPos, 1.0);
		}
	)";

	const char* fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;

		in vec3 FragPos;
		in vec3 Normal;

	    uniform vec3 lightPos;
		uniform vec3 lightAmbient;
		uniform vec3 lightDiffuse;
		uniform vec3 lightSpecular;

		uniform vec3 materialAmbient;
		uniform vec3 materialDiffuse;
		uniform vec3 materialSpecular;
		uniform float materialShininess;

		uniform vec3 viewPos;

		void main(){
			vec3 ambient = lightAmbient * materialAmbient;

			vec3 norm = normalize(Normal);
			vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
			vec3 diffuse = lightDiffuse * (materialDiffuse * diff);

	        vec3 viewDir = normalize(viewPos - FragPos);
			vec3 reflectDir = reflect(lightDir, norm);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
			vec3 specular = lightSpecular * (materialSpecular * spec);

			vec3 result = ambient + diffuse + specular;
			FragColor = vec4(result, 1.0);
		}
	)";

	static Renderer& getInstance() {
		static Renderer instance;
		return instance;
	}

	//void loadModel(const std::string& path) {
	//	auto gameObject = std::make_unique<GameObject>();
	//	Assimp::Importer importer;
	//	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
	//		std::cout << "Error from ASSIMP: " << importer.GetErrorString() << std::endl;
	//		return;
	//	}

	//	processNode(scene->mRootNode, scene, gameObject.get());
	//	gameObjects.push_back(std::move(gameObject));
	//}

	//void processNode(aiNode* node, const aiScene* scene, GameObject* gameObject) {
	//	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
	//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
	//		processMesh(mesh, scene, gameObject);
	//	}

	//	for (unsigned int i = 0; i < node->mNumChildren; i++) {
	//		processNode(node->mChildren[i], scene, gameObject);
	//	}
	//}

	//void processMesh(aiMesh* mesh, const aiScene* scene, GameObject* gameObject) {
	//	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
	//		gameObject->vertices.push_back(mesh->mVertices[i].x);
	//		gameObject->vertices.push_back(mesh->mVertices[i].y);
	//		gameObject->vertices.push_back(mesh->mVertices[i].z);

	//		if (mesh->mNormals) {
	//			gameObject->vertices.push_back(mesh->mNormals[i].x);
	//			gameObject->vertices.push_back(mesh->mNormals[i].y);
	//			gameObject->vertices.push_back(mesh->mNormals[i].z);
	//		}
	//	}

	//	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
	//		aiFace face = mesh->mFaces[i];
	//		for (unsigned int j = 0; j < face.mNumIndices; j++) {
	//			gameObject->indices.push_back(face.mIndices[j]);
	//		}
	//	}

	//	// Setup OpenGL buffers
	//	glGenVertexArrays(1, &gameObject->VAO);
	//	glGenBuffers(1, &gameObject->VBO);
	//	glGenBuffers(1, &gameObject->EBO);

	//	glBindVertexArray(gameObject->VAO);

	//	glBindBuffer(GL_ARRAY_BUFFER, gameObject->VBO);
	//	glBufferData(GL_ARRAY_BUFFER, gameObject->vertices.size() * sizeof(float), &gameObject->vertices[0], GL_STATIC_DRAW);

	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gameObject->EBO);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, gameObject->indices.size() * sizeof(unsigned int), &gameObject->indices[0], GL_STATIC_DRAW);

	//	// Position attribute
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//	glEnableVertexAttribArray(0);

	//	// Normal attribute
	//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//	glEnableVertexAttribArray(1);

	//	glBindVertexArray(0);
	//}

	void initializeShader(const char* vertexShaderSource, const char* fragmentShaderSource) {
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	glm::mat4 getView() { return view; }

	glm::mat4 getProjection() { return projection; }

	void setView(glm::mat4 view) { this->view = view; }

	void setProjection(glm::mat4 projection) { this->projection = projection; }

	void passMatricesToShader(GameObject& gameObject, Light& light) {
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(gameObject.transform.getMatrix()));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(getView()));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(getProjection()));

		// Position the light at the top-right
		glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), light.transform.position[0], light.transform.position.y, light.transform.position.z);
		// Set light colors to a cool blue
		glUniform3f(glGetUniformLocation(shaderProgram, "lightAmbient"), light.material.ambient[0], light.material.ambient[1], light.material.ambient[2]);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightDiffuse"), light.material.diffuse[0], light.material.diffuse[1], light.material.diffuse[2]);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightSpecular"), light.material.specular[0], light.material.specular[1], light.material.specular[2]);

		// Optionally, set material to a neutral or contrasting color
		glUniform3f(glGetUniformLocation(shaderProgram, "materialAmbient"), gameObject.material.ambient.x, gameObject.material.ambient.y, gameObject.material.ambient.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "materialDiffuse"), gameObject.material.diffuse.x, gameObject.material.diffuse.y, gameObject.material.diffuse.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "materialSpecular"), gameObject.material.specular.x, gameObject.material.specular.y, gameObject.material.specular.z);
		glUniform1f(glGetUniformLocation(shaderProgram, "materialShininess"), gameObject.material.shininess);

		glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), 0.0f, 0.0f, -6.0f);
	}

	void renderModel(GameObject& gameObject) {
		glBindVertexArray(gameObject.VAO);
		glDrawElements(GL_TRIANGLES, gameObject.indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void render(std::vector<std::shared_ptr<GameObject>>& gameObjects, std::vector<std::shared_ptr<Light>>& lights) {
		glUseProgram(shaderProgram);

		for (auto& gameObject : gameObjects) {
			passMatricesToShader(*gameObject, *lights[0]);  // Dereference the unique_ptr to pass a reference to GameObject
			renderModel(*gameObject);           // Dereference the unique_ptr to pass a reference to GameObject
		}
	}

private:
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(UserInterfaceManager::getInstance().sceneWindow.width / UserInterfaceManager::getInstance().sceneWindow.height), 0.1f, 100.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f));

	Renderer() {}

	Renderer(const Renderer&) = delete;

	Renderer& operator=(const Renderer&) = delete;
	~Renderer() {
		glDeleteProgram(shaderProgram);
	}
};

#endif
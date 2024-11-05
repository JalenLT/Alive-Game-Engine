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

class Renderer {
public:
	unsigned int shaderProgram = 0;

	unsigned int VAO, VBO, EBO;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

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
			vec3 reflectDir = reflect(-lightDir, norm);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
			vec3 specular = lightSpecular * (materialSpecular * spec);

			vec3 result = ambient + diffuse + specular;
			FragColor = vec4(result, 1.0);
		}
	)";

	void loadModel(const std::string& path) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "Error from ASSIMP: " << importer.GetErrorString() << std::endl;
			return;
		}

		processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode* node, const aiScene* scene) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(mesh, scene);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}

	void processMesh(aiMesh* mesh, const aiScene* scene) {
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			vertices.push_back(mesh->mVertices[i].x);
			vertices.push_back(mesh->mVertices[i].y);
			vertices.push_back(mesh->mVertices[i].z);

			if (mesh->mNormals) {
				vertices.push_back(mesh->mNormals[i].x);
				vertices.push_back(mesh->mNormals[i].y);
				vertices.push_back(mesh->mNormals[i].z);
			}
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		// Setup OpenGL buffers
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

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

	glm::mat4 getModel() { return model; }

	glm::mat4 getView() { return view; }

	glm::mat4 getProjection() { return projection; }

	void setModel(glm::mat4 model) { this->model = model; }

	void setView(glm::mat4 view) { this->view = view; }

	void setProjection(glm::mat4 projection) { this->projection = projection; }

	void passMatricesToShader() {
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(getModel()));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(getView()));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(getProjection()));

		glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), 0.0f, -2.0f, -4.0f);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightAmbient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightDiffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightSpecular"), 1.0f, 1.0f, 1.0f);

		glUniform3f(glGetUniformLocation(shaderProgram, "materialAmbient"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(shaderProgram, "materialDiffuse"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(shaderProgram, "materialSpecular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(shaderProgram, "materialShininess"), 10.0f);

		glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), 0.0f, 0.0f, -6.0f);
	}

	void renderModel() {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	Renderer(): VAO(0), VBO(0), EBO(0), model(glm::mat4(1.0f)), view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f))), projection(glm::perspective(glm::radians(45.0f), (float)(UserInterfaceManager::getInstance().sceneWindow.width / UserInterfaceManager::getInstance().sceneWindow.height), 0.1f, 100.0f)) {}

	~Renderer() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteProgram(shaderProgram);
	}

private:
	glm::mat4 model, view, projection;
};

#endif
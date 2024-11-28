#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "Observer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "assimp/scene.h"
#include "GameObject.h"
#include "Light.h"
#include <vector>
#include <memory>

class Renderer {
public:
	std::vector<std::unique_ptr<GameObject>> gameObjects;

	GLuint defaultShader{ 0 };
	GLuint lineShader{ 0 };

	static Renderer& getInstance();

	glm::mat4 getView();

	glm::mat4 getProjection();

	void setView(glm::mat4 view);

	void setProjection(glm::mat4 projection);

	void prepareShaderData(GLuint shaderProgram, Light& light);

	void prepareShaderData(GLuint shaderProgram, SubMesh& subMesh);

	void prepareShaderData(GLuint shaderProgram, Transform& transform);

	void renderModel(SubMesh& subMesh);

	void render(std::vector<std::shared_ptr<GameObject>>& gameObjects, std::vector<std::shared_ptr<Light>>& lights);

	void renderMesh(Mesh& mesh, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

	std::string readShaderSource(const char* shaderPath);

	GLuint initializeShader(const char* shaderSource, GLenum shaderType);

	GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath);

private:
	glm::mat4 projection;
	glm::mat4 view;

	Renderer();

	Renderer(const Renderer&) = delete;

	Renderer& operator=(const Renderer&) = delete;
	~Renderer();
};

#endif
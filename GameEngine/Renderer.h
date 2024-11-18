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
	unsigned int shaderProgram;
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

	std::vector<std::unique_ptr<GameObject>> gameObjects;

	static Renderer& getInstance();

	void initializeShader(const char* vertexShaderSource, const char* fragmentShaderSource);

	glm::mat4 getView();

	glm::mat4 getProjection();

	void setView(glm::mat4 view);

	void setProjection(glm::mat4 projection);

	void passMatricesToShader(GameObject& gameObject, Light& light);

	void renderModel(GameObject& gameObject);

	void render(std::vector<std::shared_ptr<GameObject>>& gameObjects, std::vector<std::shared_ptr<Light>>& lights);

private:
	glm::mat4 projection;
	glm::mat4 view;

	Renderer();

	Renderer(const Renderer&) = delete;

	Renderer& operator=(const Renderer&) = delete;
	~Renderer();
};

#endif
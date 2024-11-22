#include "Renderer.h"
#include "UserInterfaceManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

Renderer::Renderer()
	: projection(glm::perspective(glm::radians(45.0f), (float)(UserInterfaceManager::getInstance().sceneWindow.width / UserInterfaceManager::getInstance().sceneWindow.height), 0.1f, 100.0f)),
	view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f))) {}

// Destructor
Renderer::~Renderer() {}

Renderer& Renderer::getInstance() {
	static Renderer instance;
	return instance;
}

glm::mat4 Renderer::getView() { return view; }

glm::mat4 Renderer::getProjection() { return projection; }

void Renderer::setView(glm::mat4 view) { this->view = view; }

void Renderer::setProjection(glm::mat4 projection) { this->projection = projection; }

void Renderer::passMatricesToShader(GLuint shaderProgram, GameObject& gameObject, Light& light) {
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

void Renderer::renderModel(unsigned int VAO, std::vector<unsigned int> indices) {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Renderer::render(std::vector<std::shared_ptr<GameObject>>& gameObjects, std::vector<std::shared_ptr<Light>>& lights) {
	glUseProgram(defaultShader);

	// First pass: Render the Phong-shaded cube
	for (auto& gameObject : gameObjects) {
		passMatricesToShader(defaultShader, *gameObject, *lights[0]);
		renderModel(gameObject->mesh.VAO, gameObject->mesh.indices);
	}

	// Optionally, reset to no shader after rendering
	glUseProgram(0);
}

void Renderer::renderMesh(Mesh& mesh, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(lineShader);

    glDisable(GL_DEPTH_TEST);

    // Pass the model, view, and projection matrices to the shader
    GLint modelLoc = glGetUniformLocation(lineShader, "model");
    GLint viewLoc = glGetUniformLocation(lineShader, "view");
    GLint projectionLoc = glGetUniformLocation(lineShader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Set the line color
    GLint colorLocation = glGetUniformLocation(lineShader, "lineColor");
    glUniform3f(colorLocation, 0.0f, 1.0f, 0.0f); // Green color

    glBindVertexArray(mesh.VAO);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mesh.vertices.size() / 3));
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    // Optionally reset the shader program
    glUseProgram(0);
}

std::string Renderer::readShaderSource(const char* shaderPath) {
	std::ifstream shaderStream(shaderPath, std::ios::in);
	std::stringstream shaderCode;
	if (shaderStream.is_open()) {
		shaderCode << shaderStream.rdbuf();
		shaderStream.close();
	}
	else {
		std::cerr << "Could not open shader file: " << shaderPath << std::endl;
	}
	return shaderCode.str();
}

GLuint Renderer::initializeShader(const char* shaderSource, GLenum shaderType) {
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "Error compiling shader: " << infoLog << std::endl;
	}

	return shader;
}

GLuint Renderer::createShaderProgram(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode = readShaderSource(vertexPath);
	std::string fragmentCode = readShaderSource(fragmentPath);

	GLuint vertexShader = initializeShader(vertexCode.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShader = initializeShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "Error linking program: " << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
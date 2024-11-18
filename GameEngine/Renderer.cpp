#include "Renderer.h"
#include "UserInterfaceManager.h"

Renderer::Renderer()
	: shaderProgram(0),
	projection(glm::perspective(glm::radians(45.0f), (float)(UserInterfaceManager::getInstance().sceneWindow.width / UserInterfaceManager::getInstance().sceneWindow.height), 0.1f, 100.0f)),
	view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f))) {}

// Destructor
Renderer::~Renderer() {
	glDeleteProgram(shaderProgram);
}

Renderer& Renderer::getInstance() {
	static Renderer instance;
	return instance;
}

void Renderer::initializeShader(const char* vertexShaderSource, const char* fragmentShaderSource) {
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

glm::mat4 Renderer::getView() { return view; }

glm::mat4 Renderer::getProjection() { return projection; }

void Renderer::setView(glm::mat4 view) { this->view = view; }

void Renderer::setProjection(glm::mat4 projection) { this->projection = projection; }

void Renderer::passMatricesToShader(GameObject& gameObject, Light& light) {
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

void Renderer::renderModel(GameObject& gameObject) {
	glBindVertexArray(gameObject.VAO);
	glDrawElements(GL_TRIANGLES, gameObject.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Renderer::render(std::vector<std::shared_ptr<GameObject>>& gameObjects, std::vector<std::shared_ptr<Light>>& lights) {
	glUseProgram(shaderProgram);

	for (auto& gameObject : gameObjects) {
		passMatricesToShader(*gameObject, *lights[0]);  // Dereference the unique_ptr to pass a reference to GameObject
		renderModel(*gameObject);           // Dereference the unique_ptr to pass a reference to GameObject
	}
}

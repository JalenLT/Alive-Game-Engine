#include "Window.h"
#include "UserInterfaceManager.h"
#include "EventManager.h"
#include "Renderer.h"

int main() {
    try {
        Window::getInstance().initialize(1000, 800, "Alive");

        EventManager& eventManager = EventManager::getInstance();

        GLFWwindow* window = Window::getInstance().getWindow();
        UserInterfaceManager::getInstance().initialize(window);
        UserInterfaceManager::getInstance().editorLayout();

        Renderer renderer;

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &renderer.vertexShaderSource, NULL);
		glCompileShader(vertexShader);

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &renderer.fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        renderer.loadModel("C:/Users/sseunarine/OneDrive/Desktop/cube.obj");

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            UserInterfaceManager::getInstance().newFrame();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(shaderProgram);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
            glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

            // Pass matrices to the shader
            unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
            unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
            unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

            // Bind the VAO and render the model
            glBindVertexArray(renderer.VAO);
            glDrawElements(GL_TRIANGLES, renderer.indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            UserInterfaceManager::getInstance().render();

            glfwSwapBuffers(window);
        }

        glDeleteVertexArrays(1, &renderer.VAO);
        glDeleteBuffers(1, &renderer.VBO);
        glDeleteBuffers(1, &renderer.EBO);
        glDeleteProgram(shaderProgram);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

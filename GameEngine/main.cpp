#include "Window.h"
#include "UserInterfaceManager.h"
#include "EventManager.h"
#include "Renderer.h"

int main() {
    try {
        float currentFrame = 0.0f;
        float lastFrame = 0.0f;
        float maxDeltaTime = 0.05f;

        Window::getInstance().initialize(1000, 800, "Alive");

        EventManager& eventManager = EventManager::getInstance();

        GLFWwindow* window = Window::getInstance().getWindow();
        UserInterfaceManager::getInstance().initialize(window);
        UserInterfaceManager::getInstance().editorLayout();

        Renderer& renderer = Renderer::getInstance();
        renderer.initializeShader(renderer.vertexShaderSource, renderer.fragmentShaderSource);

        renderer.loadModel("C:\\Users\\sseunarine\\OneDrive\\Desktop\\cube.obj");
        //renderer.loadModel("C:\\Users\\USER\\OneDrive\\Documents\\OneDrive\\Desktop\\cube-Jalen.obj");
        //renderer.loadModel("C:/Users/sseunarine/OneDrive/Desktop/cylinder.obj");
        renderer.gameObjects[0].get()->modelMatrix = glm::translate(renderer.gameObjects[0].get()->modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        //renderer.gameObjects[1].get()->modelMatrix = glm::translate(renderer.gameObjects[1].get()->modelMatrix, glm::vec3(2.0f, 0.0f, 0.0f));

        while (!glfwWindowShouldClose(window)) {
            currentFrame = glfwGetTime();
            float deltaTime = std::min(currentFrame - lastFrame, maxDeltaTime);
            lastFrame = currentFrame;

            glfwPollEvents();
            UserInterfaceManager::getInstance().newFrame();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderer.gameObjects[0].get()->modelMatrix = glm::rotate(renderer.gameObjects[0].get()->modelMatrix, deltaTime * 1.0f, glm::vec3(0.5f, 1.0f, 0.0f));
            //renderer.gameObjects[1].get()->modelMatrix = glm::rotate(renderer.gameObjects[1].get()->modelMatrix, deltaTime * 1.0f, glm::vec3(-0.5f, -1.0f, 0.0f));

            renderer.render();

            UserInterfaceManager::getInstance().render();

            glfwSwapBuffers(window);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

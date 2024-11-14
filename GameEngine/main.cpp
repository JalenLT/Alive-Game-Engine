#include "Window.h"
#include "UserInterfaceManager.h"
#include "EventManager.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "serialization_utils.h"

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

        SceneManager::getInstance().initialize();
        renderer.initializeShader(renderer.vertexShaderSource, renderer.fragmentShaderSource);
        SceneManager::getInstance().currentScene->addGameObject("C:\\Users\\sseunarine\\OneDrive\\Desktop\\cube.obj", "Cube Original");
        SceneManager::getInstance().currentScene->addLight("Point", glm::vec3(-2.0f, 0.0f, 2.0f));

        if (std::filesystem::exists("C:\\Users\\sseunarine\\AppData\\Roaming\\Alive\\scene_save.json")) {
            SceneManager::getInstance().loadSceneFromFile("C:\\Users\\sseunarine\\AppData\\Roaming\\Alive\\scene_save.json");

            EventData data{ EventType::RefreshSceneHierarchy };

            EventManager::getInstance().notifyObservers(data);
        }

        while (!glfwWindowShouldClose(window)) {
            currentFrame = glfwGetTime();
            float deltaTime = std::min(currentFrame - lastFrame, maxDeltaTime);
            lastFrame = currentFrame;

            glfwPollEvents();

            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
                glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                std::cout << "Ctrl + S pressed, saving scene..." << std::endl;
                saveToFile(*SceneManager::getInstance().currentScene, "C:\\Users\\sseunarine\\AppData\\Roaming\\Alive\\scene_save.json", "Scene");
            }

            UserInterfaceManager::getInstance().newFrame();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderer.render(SceneManager::getInstance().currentScene->gameObjects, SceneManager::getInstance().currentScene->lights);

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

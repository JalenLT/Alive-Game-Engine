#define GLM_ENABLE_EXPERIMENTAL
#include "Window.h"
#include "UserInterfaceManager.h"
#include "EventManager.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "serialization_utils.h"
#include <thread>
#include <chrono>

int main() {
    try {
        double targetFrameRate = 120.0;
        double frameDuration = 1.0 / targetFrameRate;

        Window::getInstance().initialize(1000, 800, "Alive");

        EventManager& eventManager = EventManager::getInstance();
        GLFWwindow* window = Window::getInstance().getWindow();
        UserInterfaceManager::getInstance().initialize(window);
        //UserInterfaceManager::getInstance().editorLayout();
        UserInterfaceManager::getInstance().playbackLayout();
        Renderer& renderer = Renderer::getInstance();
        SceneManager& sceneManager = SceneManager::getInstance();

        renderer.defaultShader = renderer.createShaderProgram(
            "C:\\Users\\sseunarine\\AppData\\Roaming\\Alive\\Shaders\\defaultVertexShader.glsl",
            "C:\\Users\\sseunarine\\AppData\\Roaming\\Alive\\Shaders\\defaultFragmentShader.glsl"
        );

        renderer.lineShader = renderer.createShaderProgram(
            "C:\\Users\\sseunarine\\AppData\\Roaming\\Alive\\Shaders\\lineVertexShader.glsl",
            "C:\\Users\\sseunarine\\AppData\\Roaming\\Alive\\Shaders\\lineFragmentShader.glsl"
        );

        sceneManager.initialize();
        sceneManager.currentScene->addGameObject("C:\\Users\\sseunarine\\OneDrive\\Desktop\\cube.obj", "Cube Original");
        sceneManager.currentScene->addLight("Point", glm::vec3(-2.0f, 0.0f, 2.0f));

        if (std::filesystem::exists("C:\\Users\\sseunarine\\AppData\\Roaming\\Alive\\scene_save.json")) {
            sceneManager.loadSceneFromFile("C:\\Users\\sseunarine\\AppData\\Roaming\\Alive\\scene_save.json");

            EventData data{ EventType::RefreshSceneHierarchy };

            EventManager::getInstance().notifyObservers(data);
        }

        //sceneManager.currentScene->gameObjects[0]->boundingBox.computeBoundingBoxVertices();
        //Mesh lineMesh{};
        //lineMesh.vertices = {
        //    -2.0f, -2.0f, 0.0f,  // Vertex 1
        //     2.0f,  2.0f, 0.0f   // Vertex 2
        //};
        //lineMesh.initializeBuffers(lineMesh.vertices);

        //for (const auto& vertex : lineMesh.vertices) {
        //    std::cout << vertex << std::endl;
        //}

        //std::cout << lineMesh.vertices.size() << std::endl;

        while (!glfwWindowShouldClose(window)) {
            auto frameStartTime = std::chrono::steady_clock::now();
            Window::getInstance().updateMousePosition();

            glfwPollEvents();

            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
                glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                std::cout << "Ctrl + S pressed, saving scene..." << std::endl;
                saveToFile(*sceneManager.currentScene, "C:\\Users\\sseunarine\\AppData\\Roaming\\Alive\\scene_save.json", "Scene");
            }

            UserInterfaceManager::getInstance().newFrame();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            SceneManager::getInstance().currentScene->gameObjects[0]->transform.rotateAroundAxisAngle(glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f)), 0.1f);

            renderer.render(SceneManager::getInstance().currentScene->gameObjects, SceneManager::getInstance().currentScene->lights);
            renderer.renderMesh(sceneManager.currentScene->gameObjects[0]->boundingBox.mesh, sceneManager.currentScene->gameObjects[0]->transform.getMatrix(), renderer.getView(), renderer.getProjection());

            UserInterfaceManager::getInstance().render();

            glfwSwapBuffers(window);

            auto frameEndTime = std::chrono::steady_clock::now();

            std::chrono::duration<double> frameTime = frameEndTime - frameStartTime;
            double sleepTime = frameDuration - frameTime.count();

            if (sleepTime > 0) {
				std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

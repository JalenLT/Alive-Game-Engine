#define GLM_ENABLE_EXPERIMENTAL
#include "Window.h"
#include "UserInterfaceManager.h"
#include "EventManager.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "serialization_utils.h"
#include <thread>
#include <chrono>
#include "project_constants.h"

int main() {
    try {
        double targetFrameRate = 120.0;
        double frameDuration = 1.0 / targetFrameRate;

        Window::getInstance().initialize(1000, 800, "Alive");

        EventManager& eventManager = EventManager::getInstance();
        GLFWwindow* window = Window::getInstance().getWindow();
        UserInterfaceManager::getInstance().initialize(window);
        UserInterfaceManager::getInstance().editorLayout();
        //UserInterfaceManager::getInstance().playbackLayout();
        Renderer& renderer = Renderer::getInstance();
        SceneManager& sceneManager = SceneManager::getInstance();

        eventManager.addObserver(&sceneManager);

        renderer.defaultShader = renderer.createShaderProgram(
            (ROAMING_PATH + "\\" + PROJECT_NAME + "\\Shaders\\defaultVertexShader.glsl").c_str(),
            (ROAMING_PATH + "\\" + PROJECT_NAME + "\\Shaders\\defaultFragmentShader.glsl").c_str()
        );

        renderer.lineShader = renderer.createShaderProgram(
            (ROAMING_PATH + "\\" + PROJECT_NAME + "\\Shaders\\lineVertexShader.glsl").c_str(),
            (ROAMING_PATH + "\\" + PROJECT_NAME + "\\Shaders\\lineFragmentShader.glsl").c_str()
        );

        sceneManager.initialize();
        sceneManager.currentScene->addLight("Point", glm::vec3(-2.0f, 0.0f, 2.0f));

        if (std::filesystem::exists(ROAMING_PATH + "\\" + PROJECT_NAME + "\\scene_save.json")) {
            sceneManager.loadSceneFromFile(ROAMING_PATH + "\\" + PROJECT_NAME + "\\scene_save.json");

            EventData data{ EventType::RefreshSceneHierarchy };

            EventManager::getInstance().notifyObservers(data);
        }

        /****************************************************************************************************
         *** CREATING AND SAVING MESHES HERE (TO BE REMOVE ONCE A BASIC MESH CREATOR TOOL IS IMPLEMENTED) ***
         ****************************************************************************************************/
        Material greenMaterial;
        greenMaterial.diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
        Material redMaterial;
		redMaterial.diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
        Material blueMaterial;
		blueMaterial.diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
        while (!glfwWindowShouldClose(window)) {
            auto frameStartTime = std::chrono::steady_clock::now();
            Window::getInstance().updateMousePosition();

            glfwPollEvents();

            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
                glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                std::cout << "Ctrl + S pressed, saving scene..." << std::endl;
                saveToFile(*sceneManager.currentScene, ROAMING_PATH + "\\" + PROJECT_NAME + "\\scene_save.json", "Scene");
            }

            UserInterfaceManager::getInstance().newFrame();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderer.render(SceneManager::getInstance().currentScene->gameObjects, SceneManager::getInstance().currentScene->lights);
            for (const auto& gameObject : SceneManager::getInstance().currentScene->gameObjects) {
                renderer.renderMesh(gameObject->boundingBox.mesh, gameObject->transform.getMatrix(), renderer.getView(), renderer.getProjection(), greenMaterial);
            }

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

#include "Window.h"
#include "UserInterfaceManager.h"
#include "EventManager.h"

int main() {
    try {
        Window::getInstance().initialize(0, 0, "Alive");

        EventManager& eventManager = EventManager::getInstance();

        GLFWwindow* window = Window::getInstance().getWindow();
        UserInterfaceManager::getInstance().initialize(window);
        UserInterfaceManager::getInstance().editorLayout();

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            UserInterfaceManager::getInstance().newFrame();

            glClear(GL_COLOR_BUFFER_BIT);

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

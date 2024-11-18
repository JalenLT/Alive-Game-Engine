#include "UserInterfaceManager.h"
#include "Window.h"

UserInterfaceManager::UserInterfaceManager()
	: inspectorWindow(Window::getInstance().getWindowWidth() - (Window::getInstance().getWindowWidth() * 0.2f), 0.0f, Window::getInstance().getWindowWidth() * 0.2f, Window::getInstance().getWindowHeight() * 1.0f),
	hierarchyWindow(0.0f, 0.0f, Window::getInstance().getWindowWidth() * 0.2f, Window::getInstance().getWindowHeight() * 0.65f),
	fileBrowserWindow(0.0f, Window::getInstance().getWindowHeight() * 0.65f, Window::getInstance().getWindowWidth() * 0.8f, Window::getInstance().getWindowWidth() * 0.35f),
	sceneWindow(Window::getInstance().getWindowWidth() * 0.2f, 0.0f, Window::getInstance().getWindowWidth() * 0.6f, Window::getInstance().getWindowHeight() * 0.65f) {}

UserInterfaceManager::~UserInterfaceManager() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

UserInterfaceManager& UserInterfaceManager::getInstance() {
	static UserInterfaceManager instance;
	return instance;
}

void UserInterfaceManager::initialize(GLFWwindow* window) {
	try {
		if (!window) {
			throw std::runtime_error("Failed to initialize ImGUI: GLFW Window has not been initialized");
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
}

void UserInterfaceManager::newFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UserInterfaceManager::render() {
	for (auto& window : windows) {
		window->render();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UserInterfaceManager::addWindow(UIWindow* window) {
	windows.push_back(window);
}

void UserInterfaceManager::editorLayout() {
	addWindow(&inspectorWindow);
	addWindow(&hierarchyWindow);
	addWindow(&fileBrowserWindow);
	addWindow(&sceneWindow);

	EventManager::getInstance().addObserver(&sceneWindow);
	EventManager::getInstance().addObserver(&inspectorWindow);
	EventManager::getInstance().addObserver(&hierarchyWindow);
}
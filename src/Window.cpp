#include "Window.hpp"

Window::Window(const int width, const int height, char* title)
{
	if(!glfwInit()){
		std::cout << "WARNING: failed to initialise glfw" << std::endl;
	}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if(!window){
		glfwTerminate();
		std::cout << "WARNING: failed to create window" << std::endl;
	}

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

	glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
};

Window::~Window()
{
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
};

bool Window::isOpen()
{
    return !glfwWindowShouldClose(window);
};
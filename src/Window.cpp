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

    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowUserPointer(window, this);
};

Window::~Window()
{
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
};

void Window::keyCallback(GLFWwindow* glWindow, int key, int scancode, int action, int mods)
{
    Window* staticWindow = static_cast<Window*>(glfwGetWindowUserPointer(glWindow));

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(glWindow, 1);
    }
    else if(key >= 0 && key <= 349)
    {
        if(action == GLFW_PRESS){staticWindow->keys[key] = true;}
        if(action == GLFW_RELEASE){staticWindow->keys[key] = false;}
    }
}

bool Window::isOpen()
{
    return !glfwWindowShouldClose(window);
};
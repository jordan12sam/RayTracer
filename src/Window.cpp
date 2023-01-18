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

	glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
};

Window::~Window()
{
    glfwTerminate();
};

bool Window::isOpen()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
    return !glfwWindowShouldClose(window);
};
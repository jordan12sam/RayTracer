#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Wrapper.hpp"

#include <iostream>
#include <string>

class Window
{
    private:
        GLFWwindow* window;

    public:
        Window(const int width, const int height, char* title);
        ~Window();

        bool isOpen();
};
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.hpp>
#include <backends/imgui_impl_glfw.hpp>

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
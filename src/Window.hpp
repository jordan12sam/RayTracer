#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <examples/opengl3_example/imgui_impl_glfw_gl3.h>

#include "Wrapper.hpp"

#include <iostream>
#include <string>

class Window
{
    private:
        GLFWwindow* window;
        bool keys[350] = {false};
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    public:
        Window(const int width, const int height, char* title);
        ~Window();

        bool isOpen();

        inline GLFWwindow* get() {return window;}
        inline bool* getKeys() {return keys;}
};
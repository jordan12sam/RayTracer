#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Wrapper.hpp"
#include "Window.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Camera
{
    private:
        float step = 0.05f;

        glm::vec3 eye = glm::vec3(0, 0, 0);
        glm::vec3 forwards = glm::vec3(0, 0, -1);
        glm::vec3 up = glm::vec3(0, 1, 0);

        glm::mat4 view = glm::lookAt(
            eye,
            eye + forwards,
            up
        );

    public:
        void takeInputs(Window* window);
        void calculateView();
        inline glm::mat4 getView() {return view;}
};
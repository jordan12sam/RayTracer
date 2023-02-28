#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "Wrapper.hpp"
#include "Window.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Camera
{
    private:
        float stepLength = 0.05f;
        float stepAngle = glm::pi<float>() / 300.0f;

        glm::vec3 eye = glm::vec3(0, 0, 10);
        glm::vec3 forwards = glm::vec3(0, 0, -1);
        glm::vec3 up = glm::vec3(0, 1, 0);

        glm::mat4 view = glm::lookAt(
            eye,
            eye + forwards,
            up
        );

        void rotate(glm::vec3 axis);

    public:
        Camera(glm::vec3 eye, glm::vec3 forwards, glm::vec3 up);
        void takeInputs(Window* window);
        void calculateView();
        inline glm::mat4 getView() {return view;}
};
#include "Camera.hpp"

Camera::Camera(glm::vec3 eyeIn, glm::vec3 forwardsIn, glm::vec3 upIn)
{
    eye = eyeIn;
    forwards = forwardsIn;
    up = upIn;
}

void Camera::takeInputs(Window* window)
{
    bool* keys = window->getKeys();
    if(keys[GLFW_KEY_W])
    {
        eye += forwards * stepLength;
    }
    if(keys[GLFW_KEY_S])
    {
        eye += forwards * stepLength * -1.0f;
    }
    if(keys[GLFW_KEY_A])
    {
        eye += glm::cross(forwards, up) * stepLength * -1.0f;
    }
    if(keys[GLFW_KEY_D])
    {
        eye += glm::cross(forwards, up) * stepLength;
    }

    if(keys[GLFW_KEY_LEFT_SHIFT])
    {
        eye += up * stepLength;
    }
    if(keys[GLFW_KEY_LEFT_CONTROL])
    {
        eye += up * stepLength * -1.0f;
    }

    if(keys[GLFW_KEY_UP])
    {
        glm::vec3 axis = glm::cross(forwards, up);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), stepAngle, axis);
        forwards = glm::vec3(rotation * glm::vec4(forwards, 0.0f));
        up = glm::vec3(rotation * glm::vec4(up, 0.0f));
    }
    if(keys[GLFW_KEY_DOWN])
    {
        glm::vec3 axis = glm::cross(forwards, up) * -1.0f;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), stepAngle, axis);
        forwards = glm::vec3(rotation * glm::vec4(forwards, 0.0f));
        up = glm::vec3(rotation * glm::vec4(up, 0.0f));
    }
    if(keys[GLFW_KEY_LEFT])
    {
        glm::vec3 axis = up;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), stepAngle, axis);
        forwards = glm::vec3(rotation * glm::vec4(forwards, 0.0f));
        up = glm::vec3(rotation * glm::vec4(up, 0.0f));
    }
    if(keys[GLFW_KEY_RIGHT])
    {
        glm::vec3 axis = up * -1.0f;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), stepAngle, axis);
        forwards = glm::vec3(rotation * glm::vec4(forwards, 0.0f));
        up = glm::vec3(rotation * glm::vec4(up, 0.0f));
    }
    if(keys[GLFW_KEY_Q])
    {
        glm::vec3 axis = forwards * -1.0f;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), stepAngle, axis);
        forwards = glm::vec3(rotation * glm::vec4(forwards, 0.0f));
        up = glm::vec3(rotation * glm::vec4(up, 0.0f));
    }
    if(keys[GLFW_KEY_E])
    {
        glm::vec3 axis = forwards;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), stepAngle, axis);
        forwards = glm::vec3(rotation * glm::vec4(forwards, 0.0f));
        up = glm::vec3(rotation * glm::vec4(up, 0.0f));
    }
    this->calculateView();
}

void Camera::calculateView()
{    
    view = glm::lookAt(
        eye,
        eye + forwards,
        up
    );
}
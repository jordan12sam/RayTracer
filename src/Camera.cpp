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
        eye += forwards * step;
    }
    if(keys[GLFW_KEY_S])
    {
        eye += forwards * step * -1.0f;
    }
    if(keys[GLFW_KEY_A])
    {
        eye += glm::cross(forwards, up) * step * -1.0f;
    }
    if(keys[GLFW_KEY_D])
    {
        eye += glm::cross(forwards, up) * step;
    }
    if(keys[GLFW_KEY_LEFT_SHIFT])
    {
        eye += up * step;
    }
    if(keys[GLFW_KEY_LEFT_CONTROL])
    {
        eye += up * step * -1.0f;
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
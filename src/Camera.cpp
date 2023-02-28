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
        this->rotate(glm::cross(forwards, up));
    }
    if(keys[GLFW_KEY_DOWN])
    {
        this->rotate(glm::cross(forwards, up) * -1.0f);
    }
    if(keys[GLFW_KEY_LEFT])
    {
        this->rotate(up);
    }
    if(keys[GLFW_KEY_RIGHT])
    {
        this->rotate(up * -1.0f);
    }
    if(keys[GLFW_KEY_Q])
    {
        this->rotate(forwards * -1.0f);
    }
    if(keys[GLFW_KEY_E])
    {
        this->rotate(forwards);
    }
    this->calculateView();
}

void Camera::rotate(glm::vec3 axis)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), stepAngle, axis);
    forwards = glm::vec3(rotation * glm::vec4(forwards, 0.0f));
    up = glm::vec3(rotation * glm::vec4(up, 0.0f));
}

void Camera::calculateView()
{    
    view = glm::lookAt(
        eye,
        eye + forwards,
        up
    );
}
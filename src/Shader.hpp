#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "./vendor/glm/glm.hpp"
#include "./vendor/glm/gtc/matrix_transform.hpp"

#include "Wrapper.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Shader
{
    private:
    unsigned int rendererID;

    int getUniformLocation(const std::string& name);

    std::string parseShader(const std::string& filepath);
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int compileShader(unsigned int type, const std::string& source);

    public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniform1f(const std::string& name, float value);
    void setUniform1i(const std::string& name, int value);
    void setUniformMat4f(const std::string& name, const glm::mat4& matrix);


};
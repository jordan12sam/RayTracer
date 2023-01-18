#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Wrapper.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Shader
{
    private:
    unsigned int rendererID;

    unsigned int getUniformLocation(const std::string& name);

    std::string parseShader(const std::string& filepath);
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int compileShader(unsigned int type, const std::string& source);

    public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

};
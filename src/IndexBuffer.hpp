#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Wrapper.hpp"

#include <iostream>

class IndexBuffer
{
private:
    unsigned int RendererID;
    unsigned int Count;
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const { return Count; }
};
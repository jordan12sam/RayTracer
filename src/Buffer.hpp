#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Wrapper.hpp"

#include <iostream>

class Buffer
{
    protected:
        unsigned int RendererID;
        unsigned int type;
        virtual unsigned int getType() const = 0;
    public:
        ~Buffer();

        void Bind() const;
        void Unbind() const;

};

class IndexBuffer: public Buffer
{
    private:
        unsigned int Count;
        virtual unsigned int getType() const;
    public:
        IndexBuffer(const unsigned int* data, unsigned int count);
        inline unsigned int GetCount() const;
};

class VertexBuffer: public Buffer
{
    private:
        virtual unsigned int getType() const;
    public:
        VertexBuffer(const void* data, unsigned int size);
};
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Wrapper.hpp"

#include <iostream>

class Buffer
{
    protected:
        unsigned int rendererID;
        unsigned int type;
        virtual unsigned int getType() const = 0;
    public:
        ~Buffer();

        void bind() const;
        void unbind() const;

};

class IndexBuffer: public Buffer
{
    private:
        unsigned int count;
        virtual unsigned int getType() const;
    public:
        IndexBuffer(const unsigned int* data, unsigned int count);
        inline unsigned int getCount() const;
};

class VertexBuffer: public Buffer
{
    private:
        virtual unsigned int getType() const;
    public:
        VertexBuffer(const void* data, unsigned int size);
};
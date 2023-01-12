#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glWrap(glGenBuffers(1, &RendererID));
    this->Bind();
    glWrap(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

}

VertexBuffer::~VertexBuffer()
{
    glWrap(glDeleteBuffers(1, &RendererID));
}

void VertexBuffer::Bind() const
{
    glWrap(glBindBuffer(GL_ARRAY_BUFFER, RendererID));
}

void VertexBuffer::Unbind() const
{
    glWrap(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
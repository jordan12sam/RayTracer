#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : Count(count)
{
    glWrap(glGenBuffers(1, &RendererID));
    this->Bind();
    glWrap(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));

}

IndexBuffer::~IndexBuffer()
{
    glWrap(glDeleteBuffers(1, &RendererID));
}

void IndexBuffer::Bind() const
{
    glWrap(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID));
}

void IndexBuffer::Unbind() const
{
    glWrap(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
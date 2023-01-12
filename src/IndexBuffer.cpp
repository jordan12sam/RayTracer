#include "Buffer.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : count(count)
{
    type = getType();
    glWrap(glGenBuffers(1, &RendererID));
    this->bind();
    glWrap(glBufferData(type, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

unsigned int IndexBuffer::getType() const
{
    return GL_ELEMENT_ARRAY_BUFFER;
}

inline unsigned int IndexBuffer::getCount() const
{
    return count;
}
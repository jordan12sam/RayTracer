#include "Buffer.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : Count(count)
{
    type = getType();
    glWrap(glGenBuffers(1, &RendererID));
    this->Bind();
    glWrap(glBufferData(type, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

unsigned int IndexBuffer::getType() const
{
    return GL_ELEMENT_ARRAY_BUFFER;
}

inline unsigned int IndexBuffer::GetCount() const
{
    return Count;
}
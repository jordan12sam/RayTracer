#include "Buffer.hpp"

VertexBuffer::VertexBuffer(const std::vector<float> data, unsigned int size)
{
    type = getType();
    glWrap(glGenBuffers(1, &rendererID));
    this->bind();
    glWrap(glBufferData(type, size, &data[0], GL_STATIC_DRAW));
}

unsigned int VertexBuffer::getType() const
{
    return GL_ARRAY_BUFFER;
}
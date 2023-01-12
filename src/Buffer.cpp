#include "Buffer.hpp"

Buffer::~Buffer()
{
    glWrap(glDeleteBuffers(1, &RendererID));
}

void Buffer::bind() const
{
    glWrap(glBindBuffer(type, RendererID));
}

void Buffer::unbind() const
{
    glWrap(glBindBuffer(type, 0));
}
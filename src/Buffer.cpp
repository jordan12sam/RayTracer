#include "Buffer.hpp"

Buffer::~Buffer()
{
    glWrap(glDeleteBuffers(1, &RendererID));
}

void Buffer::Bind() const
{
    glWrap(glBindBuffer(type, RendererID));
}

void Buffer::Unbind() const
{
    glWrap(glBindBuffer(type, 0));
}
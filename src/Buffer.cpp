#include "Buffer.hpp"

Buffer::~Buffer()
{
    glWrap(glDeleteBuffers(1, &rendererID));
}

void Buffer::bind() const
{
    glWrap(glBindBuffer(type, rendererID));
}

void Buffer::unbind() const
{
    glWrap(glBindBuffer(type, 0));
}
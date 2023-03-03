#include "VertexArray.hpp"
#include "Wrapper.hpp"

VertexArray::VertexArray()
{
    glWrap(glGenVertexArrays(1, &rendererID));
    glWrap(glBindVertexArray(rendererID));
}

VertexArray::~VertexArray()
{
    glWrap(glDeleteVertexArrays(1, &rendererID))
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    bind();
    vb.bind();
    const auto& elements = layout.getElements();
    intptr_t offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glWrap(glEnableVertexAttribArray(i));
        glWrap(glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.getStride(), (const void*)offset));
        offset += element.count * VertexBufferElement::getTypeSize(element.type);
    }
}

void VertexArray::bind() const
{
    glWrap(glBindVertexArray(rendererID))
}

void VertexArray::unbind() const
{
    glWrap(glBindVertexArray(0))
}
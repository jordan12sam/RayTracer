#include "Renderer.hpp"

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.bind();
    va.bind();
    ib.bind();

    glWrap(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear() const
{
    glWrap(glClear(GL_COLOR_BUFFER_BIT));
};
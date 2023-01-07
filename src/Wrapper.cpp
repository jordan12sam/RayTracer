#include "Wrapper.hpp"

void glClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool glLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << file << "\n" << function << "\n" << line << "\nERROR CODE:" << error << std::endl;
        return false;
    }
    return true;
}
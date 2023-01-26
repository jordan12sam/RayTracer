#include "Texture.hpp"

#include "./vendor/stb_image/stb_image.h"

Texture::Texture(const std::string &path)
    : rendererID(0), filePath(path), localBuffer(nullptr), width(0), height(0), bytesPerPixel(0)
{
    stbi_set_flip_vertically_on_load(1);
    localBuffer = stbi_load(filePath.c_str(), &width, &height, &bytesPerPixel, 4);

    glWrap(glGenTextures(1, &rendererID));
    glWrap(glBindTexture(GL_TEXTURE_2D, rendererID));

    glWrap(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glWrap(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glWrap(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glWrap(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    glWrap(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer))
    unbind();

    if (localBuffer)
        stbi_image_free(localBuffer);
}

Texture::~Texture()
{
    glWrap(glDeleteTextures(1, &rendererID));
}

void Texture::bind(unsigned int slot) const
{
    glWrap(glActiveTexture(GL_TEXTURE0 + slot));
    glWrap(glBindTexture(GL_TEXTURE_2D, rendererID));
}

void Texture::unbind(unsigned int slot) const
{
    glWrap(glBindTexture(GL_TEXTURE_2D, 0));
}

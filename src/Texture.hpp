#pragma once

#include "Wrapper.hpp"

class Texture
{
    private:
        unsigned int rendererID;
        std::string filePath;
        unsigned char *localBuffer;
        int width, height, bytesPerPixel;
    public:
        Texture(const std::string &path);
        ~Texture();

        void bind(unsigned int slot = 0) const;
        void unbind(unsigned int slot = 0) const;

        inline int getWidth() const { return width; }
        inline int getHeight() const { return height; }
};
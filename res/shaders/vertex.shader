#version 330 core

layout(location = 0) in vec4 positionCoord;
layout(location = 1) in vec4 colourCoord;
layout(location = 2) in vec2 textureCoord;

uniform mat4 MVP;

out vec2 vTextureCoord;
out vec4 vColourCoord;

void main()
{
    gl_Position = MVP * positionCoord;
    vTextureCoord = textureCoord;
    vColourCoord = colourCoord;
}
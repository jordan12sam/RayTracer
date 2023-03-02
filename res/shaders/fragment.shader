#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D uTexture;

in vec2 vTextureCoord;
in vec4 vColourCoord;

void main()
{
    vec4 textureColour = texture(uTexture, vTextureCoord);
    color = textureColour * vColourCoord;
}
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./vendor/stb_image/stb_image.h"

#include "Wrapper.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>

// define screen size
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

int main(void){

    Window window(SCR_WIDTH, SCR_HEIGHT, (char*)"RayTracer");

    // initialise glew
	if(glewInit() != GLEW_OK){
		std::cout << "ERROR: failed to initialise glew" << std::endl;
        return -1;
	}

    // define a set of 2d points
    float positions[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f
    };

    unsigned int indicies[] = {
        0, 1, 2,
        0, 3, 2
    };
    
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 2);
    layout.push(GL_FLOAT, 2);

    VertexArray va;
    va.addBuffer(vb, layout);

    IndexBuffer ib(indicies, 6);

    Shader shader("../res/shaders/vertex.shader", "../res/shaders/fragment.shader");

    Texture texture("../res/textures/wood.png");
    texture.bind();
    shader.bind();
    shader.setUniform1i("uTexture", 0);

    shader.unbind();
    vb.unbind();
    ib.unbind();
    va.unbind();

    Renderer renderer;

    float r = 0.0f;
    float i = 0.05f;

    // render loop
    while (window.isOpen())
    {
        renderer.clear();

        shader.bind();
        shader.setUniform4f("uColor", r, 0.3f, 0.8f, 1.0f);

        renderer.draw(va, ib, shader);

        if (r > 1.0f)
            i = -0.05f;
        else if (r < 0.0f)
            i = 0.05f;

        r += i;
    }

	return 0;
}



#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./vendor/stb_image/stb_image.h"
#include "./vendor/glm/glm.hpp"
#include "./vendor/glm/gtc/matrix_transform.hpp"

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
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const float AR = (float)SCR_WIDTH/(float)SCR_HEIGHT;

int main(void){

    Window window(SCR_WIDTH, SCR_HEIGHT, (char*)"RayTracer");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // define a set of 2d points
    float positions[] = {
        100.0f, 100.0f, 0.0f, 0.0f,
        200.0f, 100.0f, 1.0f, 0.0f,
        200.0f, 200.0f, 1.0f, 1.0f,
        100.0f, 200.0f, 0.0f, 1.0f
    };

    unsigned int indicies[] = {
        0, 1, 2,
        0, 3, 2
    };

    glWrap(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    glWrap(glEnable(GL_BLEND));
    
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 2);
    layout.push(GL_FLOAT, 2);

    VertexArray va;
    va.addBuffer(vb, layout);

    IndexBuffer ib(indicies, 6);

    glm::mat4 proj = glm::ortho(0.0f, (float)SCR_WIDTH,
                                0.0f, (float)SCR_HEIGHT, 
                                -1.0f, 1.0f);

    Shader shader("../res/shaders/vertex.shader", "../res/shaders/fragment.shader");
    shader.bind();
    shader.setUniform1i("uTexture", 0);
    shader.setUniformMat4f("uMVP", proj);

    Texture texture("../res/textures/wood.png");
    texture.bind();

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


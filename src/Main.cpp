#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Wrapper.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>
#include <vector>

// define screen size
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
const float AR = (float)SCR_WIDTH/(float)SCR_HEIGHT;

int main(void){

    Window window(SCR_WIDTH, SCR_HEIGHT, (char*)"RayTracer");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    Camera camera(glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

    // define a set of 2d positions + 2d texture coordinates
    float positions[][3] = {
        {-1.0f, -1.0f,  1.0f},
        { 1.0f, -1.0f,  1.0f},
        { 1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f,  1.0f},

        {-1.0f, -1.0f, -1.0f},
        { 1.0f, -1.0f, -1.0f},
        { 1.0f,  1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},

        {-1.0f, -1.0f, -1.0f},
        { 1.0f, -1.0f, -1.0f},
        { 1.0f,  1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f}
    };

    float textures[][2] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},

        {1.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f},

        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f}
    };

    unsigned int baseIndicies[] = {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        0, 4, 7,
        0, 3, 7,

        1, 5, 6,
        1, 6, 2,

        3, 2, 11,
        10, 2, 11,

        0, 1, 8,
        1, 8, 9 
    
    };

    std::vector<float> vertices;
    std::vector<int> indicies;
    int n = 5;

    //glm::mat4 translation = glm::translate 

    for(int k = 0; k < n; k++)
    {        
        for(int i = 0; i < sizeof(positions)/sizeof(positions[0]); i++)
        {
            for(int j = 0; j < sizeof(positions[0])/sizeof(positions[0][0]); j++)
            {
                vertices.push_back(positions[i][j] + (k * 3.0f));
            }

            for(int j = 0; j < sizeof(textures[0])/sizeof(textures[0][0]); j++)
            {
                vertices.push_back(textures[i][j]);
            }
        }

        for(int i = 0; i < sizeof(baseIndicies)/sizeof(baseIndicies[0]); i++)
        {
            indicies.push_back(baseIndicies[i] + (k * sizeof(positions)/sizeof(positions[0])));
        }
    }

    glWrap(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    glWrap(glEnable(GL_BLEND));
    glWrap(glEnable(GL_DEPTH_TEST))

    std::cout << indicies.size()/36 << std::endl;
    
    VertexBuffer vb(vertices, vertices.size() * sizeof(float));

    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 3);
    layout.push(GL_FLOAT, 2);

    VertexArray va;
    va.addBuffer(vb, layout);

    IndexBuffer ib(indicies, indicies.size());
    
    Shader shader("../res/shaders/vertex.shader", "../res/shaders/fragment.shader");
    shader.bind();
    shader.setUniform1i("uTexture", 0);

    Texture texture("../res/textures/wood.png");
    texture.bind();

    shader.unbind();
    vb.unbind();
    ib.unbind();
    va.unbind();

    Renderer renderer;

    // render loop
    while (window.isOpen())
    {
        ImGui_ImplGlfwGL3_NewFrame();

        renderer.clear();
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        static glm::mat4 proj = glm::perspective(45.0f, (float)SCR_WIDTH/(float)SCR_HEIGHT, 1.0f, 100000.0f);

        camera.takeInputs(&window);
        glm::mat4 view = camera.getView();

        shader.bind();

        shader.setUniform4f("uColor", 1.0f, 0.0f, 0.0f, 1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 mvp = proj * view * model;
        shader.setUniformMat4f("MVP", mvp);
        renderer.draw(va, ib, shader);
        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

	return 0;
}


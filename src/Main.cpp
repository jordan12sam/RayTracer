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
#include <cmath>

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

    // define a set of 3d positions / 3d colours / 2d textures
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

    float colours[][4] = {
        {0.0f, 0.0f,  0.0f, 1.0f},
        {0.0f, 0.0f,  0.0f, 1.0f},
        {0.0f, 0.0f,  0.0f, 1.0f},
        {0.0f, 0.0f,  0.0f, 1.0f},

        {0.0f, 0.0f,  0.0f, 1.0f},
        {0.0f, 0.0f,  0.0f, 1.0f},
        {0.0f, 0.0f,  0.0f, 1.0f},
        {0.0f, 0.0f,  0.0f, 1.0f},

        {0.0f, 0.0f,  0.0f, 1.0f},
        {0.0f, 0.0f,  0.0f, 1.0f},
        {0.0f, 0.0f,  0.0f, 1.0f},
        {0.0f, 0.0f,  0.0f, 1.0f}
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

    int vecCount = sizeof(positions)/sizeof(positions[0]);
    int vecLength = sizeof(positions[0])/sizeof(positions[0][0])
                    + sizeof(colours[0])/sizeof(colours[0][0])
                    + sizeof(textures[0])/sizeof(textures[0][0]);

    std::vector<float> vertices;
    std::vector<int> indicies;
    int n = 5;

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            for(int k = 0; k < n; k++)
            {
                // For each vector
                for(int vecIndex = 0; vecIndex < vecCount; vecIndex++)
                {
                    // Translate the position coordinates
                    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3((i - n/2)*10.0f, (j - n/2)*10.0f, (k - n/2)*10.0f));
                    glm::vec3 translatedPositions = glm::vec3(glm::vec4(positions[i][0], positions[i][1], positions[i][2], 1.0f) * translation);

                    // Push position coordinates
                    for(int coordIndex = 0; coordIndex < sizeof(positions[0])/sizeof(positions[0][0]); coordIndex++)
                    {
                        vertices.push_back(translatedPositions[coordIndex]);
                    }

                    // Push colour coordinates
                    vertices.push_back((float)i/(float)n);
                    vertices.push_back((float)j/(float)n);
                    vertices.push_back((float)k/(float)n);

                    // Push texture coordinates
                    for(int coordIndex = 0; coordIndex < sizeof(textures[0])/sizeof(textures[0][0]); coordIndex++)
                    {
                        vertices.push_back(textures[vecIndex][coordIndex]);
                    }
                }

                // Push index
                for(int index = 0; index < sizeof(baseIndicies)/sizeof(baseIndicies[0]); index++)
                {
                    vertices.push_back(baseIndicies[index] * ((i * pow(n, 2) + j*n + k) + 1));
                }
            }

        }
    }

    glWrap(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    glWrap(glEnable(GL_BLEND));
    glWrap(glEnable(GL_DEPTH_TEST))
    
    VertexBuffer vb(vertices, vecCount * vecLength * pow(n, 3) * sizeof(float));

    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 3);
    layout.push(GL_FLOAT, 4);
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

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 proj = glm::perspective(45.0f, (float)SCR_WIDTH/(float)SCR_HEIGHT, 1.0f, 100000.0f);

    // render loop
    while (window.isOpen())
    {
        ImGui_ImplGlfwGL3_NewFrame();

        renderer.clear();
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        camera.takeInputs(&window);
        glm::mat4 view = camera.getView();

        shader.bind();

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


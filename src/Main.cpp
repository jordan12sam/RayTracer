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
#include <math.h>

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

    float colours[][4] = {
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f}
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

    int positionsCount = sizeof(positions)/sizeof(positions[0]);
    int positionLength = sizeof(positions[0])/sizeof(positions[0][0]);

    int coloursCount = sizeof(colours)/sizeof(colours[0]);
    int colourLength = sizeof(colours[0])/sizeof(colours[0][0]);

    int texturesCount = sizeof(textures)/sizeof(textures[0]);
    int textureLength = sizeof(textures[0])/sizeof(textures[0][0]);

    std::vector<float> vertices;
    std::vector<int> indicies;

    int n = 50;
    float scale = 10.0f;

    //glm::mat4 translation = glm::translate 
    for(int width = 0; width < n; width++)
    {  
        for(int height = 0; height < n; height++)
        {    
            for(int depth = 0; depth < n; depth++)
            {        
                float cubePosition = width*pow(n, 2) + height*n + depth;
                float noiseX = (float)(rand() % ((int)scale * 100))/10.0f * pow(-1.0f, rand() % 2);
                float noiseY = (float)(rand() % ((int)scale * 100))/10.0f * pow(-1.0f, rand() % 2);
                float noiseZ = (float)(rand() % ((int)scale * 100))/10.0f * pow(-1.0f, rand() % 2);
                

                for(int vertexIndex = 0; vertexIndex < positionsCount; vertexIndex++)
                {
                    glm::mat4 translation = glm::translate(glm::mat4(1.0f), scale * (glm::vec3(width, height, depth) + glm::vec3(noiseX, noiseY, noiseZ) + glm::vec3((float)n / -2.0f)));
                    glm::vec3 translatedPositions = glm::vec3(translation * glm::vec4(positions[vertexIndex][0], positions[vertexIndex][1], positions[vertexIndex][2], 1.0f));

                    for(int coordIndex = 0; coordIndex < positionLength; coordIndex++)
                    {
                        vertices.push_back(translatedPositions[coordIndex]);
                    }

                    for(int coordIndex = 0; coordIndex < colourLength; coordIndex++)
                    {
                        vertices.push_back(colours[vertexIndex % coloursCount][coordIndex]);
                    }

                    for(int coordIndex = 0; coordIndex < textureLength; coordIndex++)
                    {
                        vertices.push_back(textures[vertexIndex][coordIndex]);
                    }
                }

                for(int index = 0; index < sizeof(baseIndicies)/sizeof(baseIndicies[0]); index++)
                {
                    indicies.push_back(baseIndicies[index] + (cubePosition * positionsCount));
                }
            }
        }
    }

    glWrap(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    glWrap(glEnable(GL_BLEND));
    glWrap(glEnable(GL_DEPTH_TEST))
    
    VertexBuffer vb(vertices, vertices.size() * sizeof(float));

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


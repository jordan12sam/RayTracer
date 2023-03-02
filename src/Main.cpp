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
    int n = 65;
    int counter = 0;

    //glm::mat4 translation = glm::translate 
    for(int width = 0; width < n; width++)
    {  
        for(int height = 0; height < n; height++)
        {    
            for(int depth = 0; depth < n; depth++)
            {        
                for(int vertexIndex = 0; vertexIndex < sizeof(positions)/sizeof(positions[0]); vertexIndex++)
                {
                    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3((width - n/2)*25.0f, (height - n/2)*25.0f, (depth - n/2)*25.0f));
                    glm::vec3 translatedPositions = glm::vec3(translation * glm::vec4(positions[vertexIndex][0], positions[vertexIndex][1], positions[vertexIndex][2], 1.0f));

                    for(int coordIndex = 0; coordIndex < sizeof(translatedPositions)/sizeof(translatedPositions[0]); coordIndex++)
                    {
                        vertices.push_back(translatedPositions[coordIndex]);
                    }

                    for(int coordIndex = 0; coordIndex < sizeof(textures[0])/sizeof(textures[0][0]); coordIndex++)
                    {
                        vertices.push_back(textures[vertexIndex][coordIndex]);
                    }
                }

                for(int index = 0; index < sizeof(baseIndicies)/sizeof(baseIndicies[0]); index++)
                {
                    indicies.push_back(baseIndicies[index] + ((width*pow(n, 2) + height*n + depth) * sizeof(positions)/sizeof(positions[0])));
                }

                counter++;
            }
        }
    }

    glWrap(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    glWrap(glEnable(GL_BLEND));
    glWrap(glEnable(GL_DEPTH_TEST))
    
    VertexBuffer vb(vertices, vertices.size() * sizeof(float));

    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 3);
    layout.push(GL_FLOAT, 2);

    VertexArray va;
    va.addBuffer(vb, layout);

    IndexBuffer ib(indicies, indicies.size());

    std::cout << vertices.size()/(sizeof(positions)/sizeof(positions[0])) << std::endl;
    std::cout << indicies.size()/(sizeof(baseIndicies)/sizeof(baseIndicies[0])) << std::endl;
    std::cout << counter << std::endl;
    
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


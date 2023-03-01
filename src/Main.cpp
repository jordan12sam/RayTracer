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
    float positions[] = {
        -1.0f, -1.0f,  1.0f,    0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,    1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,    1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,    0.0f, 1.0f,
        
        -1.0f, -1.0f, -1.0f,    1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,    0.0f, 0.0f,
         1.0f,  1.0f, -1.0f,    0.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,    1.0f, 1.0f
    };

    unsigned int indicies[] = {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        0, 4, 7,
        0, 3, 7,

        1, 5, 6,
        1, 6, 2,

        3, 2, 7,
        6, 2, 7,

        0, 1, 4,
        1, 4, 5 
    
    };

    glWrap(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    glWrap(glEnable(GL_BLEND));
    glWrap(glEnable(GL_DEPTH_TEST))
    
    VertexBuffer vb(positions, 5 * 8 * sizeof(float));

    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 3);
    layout.push(GL_FLOAT, 2);

    VertexArray va;
    va.addBuffer(vb, layout);

    IndexBuffer ib(indicies, 36);
    
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

        static glm::vec3 translationA(0, 0, -2);
        static glm::vec3 translationB(1.5, 1.5, -5);
        static glm::vec3 translationC(-3, -1.2, -3);

        static glm::mat4 proj = glm::perspective(45.0f, (float)SCR_WIDTH/(float)SCR_HEIGHT, 1.0f, 100.0f);

        camera.takeInputs(&window);
        glm::mat4 view = camera.getView();

        {        
            static ImVec4 colourA = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
            {        
                shader.bind();
                shader.setUniform4f("uColor", colourA.x, colourA.y, colourA.z, colourA.w);
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.setUniformMat4f("MVP", mvp);
                renderer.draw(va, ib, shader);
            }

            ImGui::SliderFloat3("TranslationA", &translationA.x, -10.0f, 10.0f);
            ImGui::ColorEdit4("ColourA\n", (float*)&colourA);

            static ImVec4 colourB = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
            {        
                shader.bind();
                shader.setUniform4f("uColor", colourB.x, colourB.y, colourB.z, colourB.w);
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.setUniformMat4f("MVP", mvp);
                renderer.draw(va, ib, shader);
            }

            ImGui::SliderFloat3("TranslationB", &translationB.x, -10.0f, 10.0f);
            ImGui::ColorEdit4("ColourB\n", (float*)&colourB);

            static ImVec4 colourC = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
            {        
                shader.bind();
                shader.setUniform4f("uColor", colourC.x, colourC.y, colourC.z, colourC.w);
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationC);
                glm::mat4 mvp = proj * view * model;
                shader.setUniformMat4f("MVP", mvp);
                renderer.draw(va, ib, shader);
            }

            ImGui::SliderFloat3("TranslationC", &translationC.x, -10.0f, 10.0f);
            ImGui::ColorEdit4("ColourC\n", (float*)&colourC);
        }
        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

	return 0;
}


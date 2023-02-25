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

    // define a set of 2d positions + 2d texture coordinates
    float positions[] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        150.0f, 0.0f, 1.0f, 0.0f,
        150.0f, 150.0f, 1.0f, 1.0f,
        0.0f, 150.0f, 0.0f, 1.0f
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
                                -500.0f, 500.0f);

    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(500, 0, 0));
    
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
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        renderer.clear();

        static glm::vec3 translationA(0, 0, 0);
        static glm::vec3 translationB(200, 200, 0);
        static glm::vec3 translationC(400, 400, 0);

        static ImVec4 colourA = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        {        
            shader.bind();
            shader.setUniform4f("uColor", colourA.x, colourA.y, colourA.z, colourA.w);
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;
            shader.setUniformMat4f("uMVP", mvp);
            renderer.draw(va, ib, shader);
        }

        ImGui::SliderFloat3("TranslationA", &translationA.x, -1280.0f, 1280.0f);
        ImGui::ColorEdit4("ColourA", (float*)&colourA);

        static ImVec4 colourB = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        {        
            shader.bind();
            shader.setUniform4f("uColor", colourB.x, colourB.y, colourB.z, colourB.w);
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            glm::mat4 mvp = proj * view * model;
            shader.setUniformMat4f("uMVP", mvp);
            renderer.draw(va, ib, shader);
        }

        ImGui::SliderFloat3("TranslationB", &translationB.x, -1280.0f, 1280.0f);
        ImGui::ColorEdit4("ColourB", (float*)&colourB);

        static ImVec4 colourC = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        {        
            shader.bind();
            shader.setUniform4f("uColor", colourC.x, colourC.y, colourC.z, colourC.w);
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationC);
            glm::mat4 mvp = proj * view * model;
            shader.setUniformMat4f("uMVP", mvp);
            renderer.draw(va, ib, shader);
        }

        ImGui::SliderFloat3("TranslationC", &translationC.x, -1280.0f, 1280.0f);
        ImGui::ColorEdit4("ColourC", (float*)&colourC);
        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window.window);
    }

	return 0;
}


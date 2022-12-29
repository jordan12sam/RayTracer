#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

// screen size
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

int main()
{
    // glfw: initialise and configure
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RayTracer", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glew: initialise
    if (glewInit() != GLEW_OK)
        return -1;

    float positions[6] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.0f, 0.5f
    };

    // vertex buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(positions[0]) * 2, 0);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        //render        
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for process events6 * sizeof(float)
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}



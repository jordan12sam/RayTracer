#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Wrapper.hpp"
#include "Renderer.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>

// define screen size
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

static std::string parseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::stringstream ss;

    std::string line;
    while (getline(stream, line))
    {
        ss << line << "\n";
    }

    return ss.str();
}

static unsigned int compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if ( result == GL_FALSE )
    {
      int length;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
      char* message = (char*)alloca(length * sizeof(char));
      glGetShaderInfoLog(id, length, &length, message);
      std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader: " << message << std::endl;
      glDeleteShader(id);
      return 0;
    }

    return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void){

    // initialise glfw
	if(!glfwInit()){
		std::cout << "ERROR: failed to initialise glfw" << std::endl;
        return -1;
	}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RayTracer", NULL, NULL);

	if(!window){
		glfwTerminate();
		std::cout << "ERROR: failed to create window" << std::endl;
		return -1;
	}

    // make the window's context current
	glfwMakeContextCurrent(window);

    // track monitor refresh rate
    glfwSwapInterval(1);

    // initialise glew
	if(glewInit() != GLEW_OK){
		std::cout << "ERROR: failed to initialise glew" << std::endl;
        return -1;
	}

    // define a set of 2d points
    float positions[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f
    };

    unsigned int indicies[] = {
        0, 1, 2,
        0, 3, 2
    };

    // vertex array
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // vertex buffer
    VertexArray va;
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 2);
    va.addBuffer(vb, layout);

    // index buffer
    IndexBuffer ib(indicies, 6);

    // shaders
    std::string vertexShader = parseShader("../res/shaders/vertex.shader");
    std::string fragmentShader = parseShader("../res/shaders/fragment.shader");

    unsigned int shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    // uniform
    int location = glGetUniformLocation(shader, "uColor");
    assert(location != -1);

    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float r = 0.0f;
    float i = 0.05f;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // clear window
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glUniform4f(location, r, 0.3f, 0.8f, 1.0f);

        va.bind();
        ib.bind();

        glWrap(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f)
            i = -0.05f;
        else if (r < 0.0f)
            i = 0.05f;

        r += i;

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for process events
        glfwPollEvents();
    }

    glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}



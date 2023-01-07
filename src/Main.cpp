#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "headers/Wrapper.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
	}

    // create a window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RayTracer", NULL, NULL);

	if(!window){
		glfwTerminate();
		std::cout << "ERROR: failed to create window" << std::endl;
		return -1;
	}

    // make the window's context current
	glfwMakeContextCurrent(window);

    // initialise glew
	if(glewInit() != GLEW_OK){
		std::cout << "ERROR: failed to initialise glew" << std::endl;
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

    // vertex buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 6 * 2, positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    // index buffer
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* 6, indicies, GL_STATIC_DRAW);

    // shaders
    std::string vertexShader = parseShader("../res/shaders/vertex.shader");
    std::string fragmentShader = parseShader("../res/shaders/fragment.shader");

    unsigned int shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // clear window
        glClear(GL_COLOR_BUFFER_BIT);

        //draw points
        glWrap(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for process events
        glfwPollEvents();
    }

    glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}



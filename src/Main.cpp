#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

// screen size
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

static unsigned int CompileShader(unsigned int type, const std::string& source)
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

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6, positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

    std::string vertexShader =
      "#version 330 core\n"
      "\n"
      "layout(location = 0) in vec4 position;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = position;\n"
      "}\n";

    std::string fragmentShader =
      "#version 330 core\n"
      "\n"
      "layout(location = 0) out vec4 color;\n"
      "void main()\n"
      "{\n"
      "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
      "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        //render        
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for process events
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}



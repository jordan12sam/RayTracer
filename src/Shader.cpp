#include "Shader.hpp"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : rendererID(0)
{
    std::string vertexSource = parseShader(vertexPath);
    std::string fragmentSource = parseShader(fragmentPath);

    rendererID = createShader(vertexSource, fragmentSource);
}

Shader::~Shader()
{
    glWrap(glDeleteProgram(rendererID));
}

std::string Shader::parseShader(const std::string& filepath)
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

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    glWrap(unsigned int program = glCreateProgram());
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glWrap(glAttachShader(program, vs));
    glWrap(glAttachShader(program, fs));
    glWrap(glLinkProgram(program));
    glWrap(glValidateProgram(program));

    glWrap(glDeleteShader(vs));
    glWrap(glDeleteShader(fs));

    return program;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    glWrap(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    glWrap(glShaderSource(id, 1, &src, nullptr));
    glWrap(glCompileShader(id));

    int result;
    glWrap(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
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

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glWrap(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::getUniformLocation(const std::string& name)
{
    glWrap(int location = glGetUniformLocation(rendererID, name.c_str()));
    if (location == -1)
        std::cout << "WARNING: Uniform " << name << " has location -1!" << std:: endl;
    return location;
}

void Shader::bind() const
{
    glWrap(glUseProgram(rendererID));
}

void Shader::unbind() const
{
    glWrap(glUseProgram(0));
}
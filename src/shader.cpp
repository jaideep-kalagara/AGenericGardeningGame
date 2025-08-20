#include "shader.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <fstream>
#include <print>
#include <sstream>
#include <string>

#include "resources.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // load shader files
    std::string vertexShaderSource = load_file(vertexPath);
    std::string fragmentShaderSource = load_file(fragmentPath);

    // create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vertexShaderSourceCStr = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSourceCStr, nullptr);
    glCompileShader(vertexShader);

    // check for compile errors
    checkShaderError(vertexShader, "Vertex Shader");

    // create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fragmentShaderSourceCStr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, nullptr);
    glCompileShader(fragmentShader);

    // check for compile errors
    checkShaderError(fragmentShader, "Fragment Shader");

    // create shader program
    // create the shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // detach and delete the shaders
    glDetachShader(ID, vertexShader);
    glDetachShader(ID, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // check for shader compile errors
    GLint success = 0;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::println(stderr, "Error shader failed to compile:\n{}\n", infoLog);
    }
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::checkShaderError(GLuint shader, std::string shaderType) {
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::println(stderr, "Error shader of type {} failed to compile:\n{}\n", shaderType, infoLog);
    }
}

GLint Shader::getUniform(std::string uniformName) {
    return glGetUniformLocation(ID, uniformName.c_str());
}
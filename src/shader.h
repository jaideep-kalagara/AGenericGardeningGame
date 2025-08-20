#pragma once

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <string>

class Shader {
   public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader() {}

    void use();

    GLint getUniform(std::string uniformName);

    unsigned int
    getID() { return ID; }

   private:
    unsigned int ID;

    void checkShaderError(GLuint shader, std::string shader_type);
};

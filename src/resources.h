#pragma once

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <fstream>
#include <print>
#include <sstream>
#include <string>

static std::string load_file(const char* path) {
    std::ifstream source(path, std::ios::in);
    std::stringstream buffer;

    if (!source) {
        std::println(stderr, "Could not open file at path {}", path);
        return "";
    }

    buffer << source.rdbuf();

    // close file and return
    source.close();
    return buffer.str();
}
